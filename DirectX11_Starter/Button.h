#ifndef _BUTTON_H
#define _BUTTON_H

#include <cstdint>
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "WindowSingleton.h"
#include "GameEntity.h"

class Button{
public:
	enum MouseState{
		Neutral = 0,
		Hover,
		Press,
		Release,
		Click
	};
	Button(wchar_t* text = L"");
	~Button();
	void update();
	MouseState getMouseState();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	void setPosition(XMFLOAT3 position);
protected:
	wchar_t* m_text;
	static DirectX::SpriteBatch* m_spriteBatch;
	static DirectX::SpriteFont* m_spriteFont;
	XMFLOAT2 getCenter(wchar_t* text);
	XMFLOAT4 m_color;
	GameEntity* m_ge;
	WindowSingleton *wind;
	MouseState m_mouseState;
	Timer m_timer;
	Timer m_pressedTimer;
	bool m_canClick;
};

#endif