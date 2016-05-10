#ifndef _MENUSTATE_H
#define _MENUSTATE_H

#include "State.h"
#include "Button.h"

class MenuState : public State{
public:
	MenuState();
	~MenuState();
	void update(float dt) override;
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
protected:
	Button* m_gameButton;
	Button* m_tutButton;
	Button* m_scoreButton;
};

#endif