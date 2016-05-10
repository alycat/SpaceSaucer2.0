#include "WinState.h"
#include <iostream>
#include <stdio.h>

char WinState::letters[] = { 'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z' };

WinState::WinState() : State(new Material(1, L"gameOverScreen.png"))
{
	m_highScoreMgr = HighScoreManager::Instance();
	m_button = new Button(L"Enter");
	str = "";
	m_timer = { 0.0f, false };
}

WinState::~WinState()
{

}

void WinState::update(float dt)
{
	if (m_timer.isRunning)
	{
		m_timer.time += 0.01;
		if (m_timer.time >= 1)
		{
			m_timer.time = 0.0f;
			m_timer.isRunning = false;
		}
	}
	else{
		for (int i = 0; i < 26; ++i)
		{
			if (GetAsyncKeyState(letters[i]) & 0x8000)
			{
				str += letters[i];
				m_timer.isRunning = true;
			}
		}
		if (GetAsyncKeyState(VK_BACK) & 0x8000)
		{
			if (std::strlen(str.c_str()) > 0)
			{
				str = str.substr(0, str.size() - 1);
				m_timer.isRunning = true;
			}
		}
	}

	m_button->update();
	if (m_button->getMouseState() == Button::Click)
	{
		m_player->name = str;
		m_highScoreMgr->writeScore();
		m_player->reset();
		m_stateMgr->gameState = Scores;
	}
}

void WinState::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	std::wstring user(str.begin(), str.end());
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch, L"Congratulations!", DirectX::SimpleMath::Vector2(0, 0), Colors::White);
	spriteFont->DrawString(spriteBatch, L"Enter name:", DirectX::SimpleMath::Vector2(0, 50), Colors::White);
	spriteFont->DrawString(spriteBatch, user.c_str(), DirectX::SimpleMath::Vector2(0, 100), Colors::White);
	spriteBatch->End();
	m_button->draw(viewMatrix, projectionMatrix, camPos, time);
}