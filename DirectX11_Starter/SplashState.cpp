#include "SplashState.h"

SplashState::SplashState() : State(new Material(1, L"nebulaedit.png"), L"SplashPixelShader.cso")
{
	m_ge->sendConstantBufferDataToShaderProgram(CBStreamout, SPPixel);
	m_canStart = false;
}

SplashState::~SplashState()
{
}

void SplashState::update(float dt)
{
	static float timer = 0.0f;
	if (timer >= 5.0f)
		m_canStart = true;
	else
		timer += dt;
	if (m_canStart && GetAsyncKeyState(VK_RETURN) & 0x8000)
		m_stateMgr->gameState = Menu;
}

void SplashState::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	State::draw(viewMatrix, projectionMatrix, camPos, (1 + sin(time/5))*20);
	if (m_canStart)
	{
		std::wstring enterText = L"Press 'Enter'";
		XMVECTOR center = spriteFont->MeasureString(enterText.c_str()) / 2;
		spriteBatch->Begin();
		spriteFont->DrawString(spriteBatch, enterText.c_str(), DirectX::SimpleMath::Vector2(wind->windowWidth/2 - center.m128_f32[0], wind->windowHeight/2 - center.m128_f32[1]), Colors::White);
		spriteBatch->End();
	}
}