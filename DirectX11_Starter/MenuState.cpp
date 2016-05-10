#include "MenuState.h"

MenuState::MenuState() : State(nullptr)
{
	m_gameButton = new Button(L"Play");
	m_gameButton->setPosition(XMFLOAT3(0, 3, 0));
	m_tutButton = new Button(L"How to\nPlay");
	m_tutButton->setPosition(XMFLOAT3(0, 0, 0));
	m_scoreButton = new Button(L"Scores");
	m_scoreButton->setPosition(XMFLOAT3(0, -3, 0));
}

MenuState::~MenuState()
{

}

void MenuState::update(float dt)
{
	m_gameButton->update();
	m_tutButton->update();
	m_scoreButton->update();
	if (m_gameButton->getMouseState() == Button::Click)
		m_stateMgr->gameState = Play;
	if (m_tutButton->getMouseState() == Button::Click)
		m_stateMgr->gameState = Tutorial;
	if (m_scoreButton->getMouseState() == Button::Click)
		m_stateMgr->gameState = Scores;

	/*if (GetAsyncKeyState('1') & 0x8000)
	{
		m_stateMgr->gameState = Play;
	}
	else if (GetAsyncKeyState('2') & 0x8000)
	{
		m_stateMgr->gameState = Tutorial;
	}*/
}

void MenuState::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	//State::draw(viewMatrix, projectionMatrix, camPos, time);
	m_gameButton->draw(viewMatrix, projectionMatrix, camPos, time);
	m_tutButton->draw(viewMatrix, projectionMatrix, camPos, time);
	m_scoreButton->draw(viewMatrix, projectionMatrix, camPos, time);
}