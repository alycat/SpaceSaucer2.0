#ifndef _WINSTATE_H
#define _WINSTATE_H

#include "State.h"
#include "Button.h"
#include <string>
#include <wchar.h>
#include "Global.h"
#include "Player.h"
#include "HighScoreManager.h"

class WinState : public State{
public:
	WinState();
	~WinState();
	void update(float dt) override;
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
protected:
	Button *m_button;
	std::string str;
	static char letters[26];
	Timer m_timer;
	Player *m_player;
	HighScoreManager* m_highScoreMgr;
};

#endif