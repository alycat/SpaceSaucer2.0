#include "Button.h"
#include "ObjectLoader.h"
#include <DirectXMath.h>

DirectX::SpriteBatch* Button::m_spriteBatch = nullptr;
DirectX::SpriteFont* Button::m_spriteFont = nullptr;

Button::Button(wchar_t* text)
	:m_text(text), m_color(0, 0.25, 0.5,1)
{
	if (!m_spriteBatch)
	{
		m_spriteBatch = new DirectX::SpriteBatch(wind->deviceContext);
	}
	if (!m_spriteFont)
	{
		m_spriteFont = new DirectX::SpriteFont(wind->device, L"chromia20.spritefont");
	}
	ObjectLoader obj;
	m_ge = new GameEntity(obj.LoadModel("fullscreenQuad.obj"), 
		new Material(1, L"button.png"), 
		new ShaderProgram(L"FlatVertexShader.cso", L"ButtonPixelShader.cso"), 10, 5);
	//m_ge->scale(XMFLOAT3(0.5, 0.5, 1));
	m_ge->scale(XMFLOAT3(0.5, 0.25, 1));
	m_ge->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_ge->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);
	//m_ge->setPosition(XMFLOAT3(2, 0, 0));
	//m_ge->translate(XMFLOAT3(5, 0, 0));
	m_mouseState = Neutral;
	m_timer = { 0.0f, false };
	m_pressedTimer = { 0.0f, false };
	m_canClick = true;
}

Button::~Button()
{

}

Button::MouseState Button::getMouseState()
{
	return m_mouseState;
}

void Button::update()
{
	if (m_pressedTimer.isRunning)
	{
		m_pressedTimer.time += 0.1;
		if (m_pressedTimer.time >= 10.0f)
		{
			m_pressedTimer.isRunning = false;
			m_pressedTimer.time = 0.0f;
			m_canClick = false;
		}
	}

	if (m_timer.isRunning)
	{
		m_timer.time += 0.1;
		if (m_timer.time >= 1.0f)
		{
			m_timer.isRunning = false;
			m_timer.time = 0.0f;
		}
	}
	else{
		POINT position;
		GetCursorPos(&position);
		ScreenToClient(GetActiveWindow(), &position);
		BoundingBox mouseBB(XMFLOAT3((position.x - wind->windowWidth / 2)/5, (position.y - wind->windowHeight / 2)/5, 1.0f), XMFLOAT3(10.0f, 10.0f, 1.0f));
		XMFLOAT3 buttonPos = m_ge->getPositionVector();
		BoundingBox buttonBB(XMFLOAT3(buttonPos.x * 13, buttonPos.y * -7, 0), XMFLOAT3(5, 1.25, 1.0));
		if (buttonBB.Contains(mouseBB))
		{
			if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
			{
				m_mouseState = Press;
				m_pressedTimer.isRunning = true;
			}
			else
			{
				if (m_mouseState == Press && m_canClick)
				{
					m_mouseState = Click;
					m_timer.isRunning = true;
				}
				else
				{
					m_mouseState = Hover;
				}
			}
		}
		else
		{
			if (m_mouseState == Press && m_canClick)
			{
				m_mouseState = Click;
				m_timer.isRunning = true;
			}
			else
				m_mouseState = Neutral;
		}
		if (m_mouseState == Neutral || m_mouseState == Hover)
		{
			m_pressedTimer.isRunning = false;
			m_pressedTimer.time = 0.0f;
			m_canClick = true;
		}
	}
}

XMFLOAT2 get2DFrom3D(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT4X4 worldMatrix, XMFLOAT3 vec3D)
{
	XMFLOAT4 pos(vec3D.x, vec3D.y, vec3D.z, 1.0f);
	XMMATRIX projection = XMLoadFloat4x4(&projectionMatrix);
	XMMATRIX view = XMLoadFloat4x4(&viewMatrix);
	XMMATRIX world = XMLoadFloat4x4(&worldMatrix);
	XMMATRIX worldProjMatrix = XMMatrixMultiply(projection, XMMatrixMultiply(world, view));
	XMVECTOR position = XMVector3Transform(XMLoadFloat3(&vec3D), worldProjMatrix);
	return XMFLOAT2(position.m128_f32[0], position.m128_f32[1]);
	//return XMFLOAT2(vec3D.x / vec3D.z, vec3D.y / vec3D.z);
}

XMFLOAT2 Button::getCenter(wchar_t* string)
{
	XMVECTOR xmCenter = m_spriteFont->MeasureString(string)/2;
	return XMFLOAT2(xmCenter.m128_f32[0], xmCenter.m128_f32[1]);
}


void Button::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	DirectX::XMVECTORF32 color;
	switch (m_mouseState)
	{
	case Neutral: {
		color = Colors::White;
		m_color = XMFLOAT4(0, 0.25, 0.5, 1);
		break;
	}
	case Hover: {
		color = Colors::Cyan;
		m_color = XMFLOAT4(0, 1, 1, 1);
		break;
	}
	case Press: { 
		color = Colors::DarkTurquoise;
		m_color = XMFLOAT4(0, 0.5, 0.5, 1);
		break;
	}
	default: {
		color = Colors::Black;
		m_color = XMFLOAT4(0, 0, 0.5, 1);
	}
	}
	LightBuffer lightData;
	lightData.diffuse = m_color;
	m_ge->lightData = lightData;
	//XMFLOAT3 flatPosition = m_ge
	if (m_spriteBatch && m_spriteFont)
	{
		XMFLOAT2 center = getCenter(m_text);
		XMFLOAT2 position = get2DFrom3D(viewMatrix, projectionMatrix, m_ge->world, m_ge->getPositionVector());
		m_ge->draw(viewMatrix, projectionMatrix, camPos, time);
		XMFLOAT2 newPos = XMFLOAT2(position.x*130 + wind->windowWidth/2, -position.y*70 + wind->windowHeight/2);
		m_spriteBatch->Begin();
		static float factor = 80;
		m_spriteFont->DrawString(m_spriteBatch, m_text, DirectX::SimpleMath::Vector2(newPos.x - center.x, newPos.y - center.y) , color);
		m_spriteBatch->End();
	}
}

void Button::setPosition(XMFLOAT3 position)
{
	m_ge->setPosition(position);
}