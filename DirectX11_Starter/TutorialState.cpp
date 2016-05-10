#include "TutorialState.h"

TutorialState::TutorialState() : State(new Material(1, L"InstructionsScreen.png"))
{
	m_menuButton = new Button(L"Back to\n Menu");
	m_menuButton->setPosition(XMFLOAT3(0, -5, 0));
}

TutorialState::~TutorialState()
{

}

void TutorialState::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	spriteBatch->Begin();
	spriteFont->DrawString(spriteBatch, L"W A S D: Move", DirectX::SimpleMath::Vector2(wind->windowWidth/4, wind->windowHeight/4 - 50), Colors::White);
	spriteFont->DrawString(spriteBatch, L"P: Pause", DirectX::SimpleMath::Vector2(wind->windowWidth/4, wind->windowHeight/4 + 50), Colors::White);
	spriteBatch->End();

	//State::draw(viewMatrix, projectionMatrix, camPos, time);
	m_menuButton->draw(viewMatrix, projectionMatrix, camPos, time);
}

void TutorialState::update(float dt)
{
	m_menuButton->update();
	if (m_menuButton->getMouseState() == Button::Click)
		m_stateMgr->gameState = Menu;

	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		m_stateMgr->gameState = Menu;
	}
}