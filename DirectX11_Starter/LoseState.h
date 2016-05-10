#ifndef _LOSESTATE_H
#define _LOSESTATE_H

#include "State.h"
#include "Button.h"

class LoseState : public State{
public:
	LoseState();
	~LoseState();
	void update(float dt) override;
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
protected:
	Button* m_button;
};
#endif