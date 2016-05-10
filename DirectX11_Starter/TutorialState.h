#ifndef _TUTORIALSTATE_H
#define _TUTORIALSTATE_H
#include "State.h"
#include "Button.h"

class TutorialState : public State{
public:
	TutorialState();
	~TutorialState();
	void update(float dt) override;
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
protected:
	Button* m_menuButton;
};
#endif