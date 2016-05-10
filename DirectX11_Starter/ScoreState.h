#ifndef _SCORESTATE_H
#define _SCORESTATE_H

#include "State.h"
#include "HighScoreManager.h"
#include "Button.h"

class ScoreState : public State
{
public:
	ScoreState();
	~ScoreState();
	void update(float dt) override;
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
protected:
	HighScoreManager* m_highScoreMgr;
	Button* m_button;
};
#endif