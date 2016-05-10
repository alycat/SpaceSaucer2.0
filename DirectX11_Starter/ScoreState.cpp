#include "ScoreState.h"

ScoreState::ScoreState() : State(new Material(1, L"gameOverScreen.png"))
{
	m_button = new Button(L"Back to\nMenu");
	m_button->setPosition(XMFLOAT3(0, 4, 0));
}

ScoreState::~ScoreState()
{

}

void ScoreState::update(float dt)
{
	m_button->update();
	if (m_button->getMouseState() == Button::Click)
		m_stateMgr->gameState = Menu;
}

void ScoreState::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	//State::draw(viewMatrix, projectionMatrix, camPos, time);
	std::vector<HighScoreManager::Score> scores = m_highScoreMgr->getHighScores();
	int y = 50;
	spriteBatch->Begin();
	for each(HighScoreManager::Score score in scores)
	{
		std::wstring scoreStr(score.user.begin(), score.user.end());
		scoreStr.append(L": ");
		scoreStr.append(std::to_wstring(score.score));
		// +" " + score.score;
		spriteFont->DrawString(spriteBatch, scoreStr.c_str(), DirectX::SimpleMath::Vector2(50, y), Colors::White);
		y += 50;
	}
	spriteBatch->End();
	m_button->draw(viewMatrix, projectionMatrix, camPos, time);
}