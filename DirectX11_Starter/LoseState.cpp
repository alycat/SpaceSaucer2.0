#include "LoseState.h"

LoseState::LoseState() : State(new Material(1, L"losestate.png"))
{
	m_button = new Button(L"Back to\nMenu");
	m_button->setPosition(XMFLOAT3(0, -1, 0));
}

LoseState::~LoseState()
{

}

void LoseState::update(float dt)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_stateMgr->gameState = Menu;
	}
	m_button->update();
	if (m_button->getMouseState() == Button::Click)
	{
		m_stateMgr->gameState = Menu;
	}
}

void LoseState::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	std::wstring loseText = L"Game Over";
	XMVECTOR center = spriteFont->MeasureString(loseText.c_str()) / 2;
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch, loseText.c_str(), DirectX::SimpleMath::Vector2(wind->windowWidth / 2 - center.m128_f32[0], wind->windowHeight / 2 - center.m128_f32[1]), Colors::White);
	spriteBatch->End();
	m_button->draw(viewMatrix, projectionMatrix, camPos, time);
}