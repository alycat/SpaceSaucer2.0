#ifndef _HIGHSCOREMANAGER_H
#define _HIGHSCOREMANAGER_H
#include <vector>
#include <string>
#include "Player.h"
#define MAXSCORES 10
class HighScoreManager{
public:
	static HighScoreManager* Instance();
	struct Score{
		int score;
		std::string user;
	};

	bool checkScores();
	void writeScore();
	std::vector<Score> getHighScores();
protected:
	HighScoreManager();
	static std::vector<Score> m_scores;
	static HighScoreManager* m_highScoreMgr;
	Player *m_player;
	int m_index;
};
#endif