#include "HighScoreManager.h"
#include <iostream>
#include <sstream>
#include <fstream>
HighScoreManager* HighScoreManager::m_highScoreMgr = nullptr;
std::vector<HighScoreManager::Score> HighScoreManager::m_scores = {};
using namespace std;

vector<string> split(string line, char delimitter)
{
	vector<string> internal;
	stringstream ss(line);
	string tok;
	while (getline(ss, tok, delimitter))
	{
		internal.push_back(tok);
	}
	return internal;
}

HighScoreManager::HighScoreManager()
{
	m_index = -1;
	string line;
	ifstream myFile("scores.txt");
	if (myFile.is_open())
	{
		while (getline(myFile, line))
		{
			vector<string> score = split(line, ',');
			m_scores.push_back(Score{ stoi(score[1]), score[0] });
			cout << line;
		}
		myFile.close();
	}
}

bool HighScoreManager::checkScores()
{
	m_index = -1;
	for (int i = m_scores.size() - 1; i >= 0; --i)
	{
		if (m_scores[i].score < m_player->score)
			m_index = i;
	}
	return m_index < 10 && m_index > -1;
}

void HighScoreManager::writeScore()
{
	m_scores.insert(m_scores.begin() + m_index, 1, Score{ m_player->score, m_player->name });
	m_scores.resize(10);
	ofstream myFile("scores.txt");
	if (myFile.is_open())
	{
		for each(Score score in m_scores)
		{
			myFile << score.user << "," << score.score << "\n";
		}
		myFile.close();
	}
}

HighScoreManager* HighScoreManager::Instance()
{
	if (!m_highScoreMgr)
	{
		m_highScoreMgr = new HighScoreManager();
	}
	return m_highScoreMgr;
}

vector<HighScoreManager::Score> HighScoreManager::getHighScores()
{
	return m_scores;
}
