#include "StateManager.h"

StateManager* StateManager::m_stateMgr = nullptr;
GameState StateManager::m_gameState = GameState::Splash;

StateManager::StateManager()
{

}

StateManager* StateManager::Instance()
{
	if (!m_stateMgr)
	{
		m_stateMgr = new StateManager();
	}
	return m_stateMgr;
}

GameState StateManager::getGameState()
{
	return m_gameState;
}

void StateManager::setGameState(GameState gameState)
{
	m_gameState = gameState;
}