#ifndef _STATE_H
#define _STATE_H

#include "GameEntity.h"
#include "Global.h"
#include "StateManager.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "WindowSingleton.h"

class State{
public:
	State(Material* mat, wchar_t *pixelShader = L"FlatPixelShader.cso");
	~State(void);
	virtual void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	virtual void update(float dt) = 0;
protected:
	ConstantBufferContainer* cb_container;
	GameEntity* m_ge;
	StateManager* m_stateMgr;

	static DirectX::SpriteBatch *spriteBatch;
	static DirectX::SpriteFont* spriteFont;
	WindowSingleton *wind;
};
#endif