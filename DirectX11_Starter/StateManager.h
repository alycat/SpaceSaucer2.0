#ifndef _STATEMANAGER_H
#define _STATEMANAGER_H

#include "Global.h"

class StateManager{
public:
	static StateManager* Instance();
	GameState getGameState();
	void setGameState(GameState gameState);
	__declspec(property(get = getGameState, put = setGameState)) GameState gameState;
protected:
	StateManager();
	static StateManager* m_stateMgr;
	static GameState m_gameState;
};

#endif