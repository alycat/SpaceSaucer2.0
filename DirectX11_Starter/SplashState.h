#ifndef _SPLASHSTATE_H
#define _SPLASHSTATE_H

#include "State.h"
#include "Button.h"
#include "StarParticleSystem.h"

class SplashState : public State{
public:
	SplashState();
	~SplashState();
	void update(float dt) override;
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
protected:
	bool m_canStart;
};
#endif