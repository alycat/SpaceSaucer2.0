#include "State.h"
#include "ObjectLoader.h"

DirectX::SpriteBatch* State::spriteBatch = nullptr;
DirectX::SpriteFont* State::spriteFont = nullptr;

State::State(Material* material, wchar_t* pixelShader)
{
	ObjectLoader obj;
	m_ge = new GameEntity(obj.LoadModel("fullscreenQuad.obj"),
		material,
		new ShaderProgram(L"FlatVertexShader.cso", pixelShader));

	m_ge->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_ge->scale(XMFLOAT3(3.75, 2.34375, 1));

	if (!spriteBatch)
		spriteBatch = new DirectX::SpriteBatch(wind->deviceContext);
	if (!spriteFont)
		spriteFont = new DirectX::SpriteFont(wind->device, L"oblivious32.spritefont");
}


State::~State(void)
{

}

void State::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	m_ge->draw(viewMatrix, projectionMatrix, camPos, time);
}