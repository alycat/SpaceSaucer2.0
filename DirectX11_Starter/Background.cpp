#include "Background.h"
#include "ObjectLoader.h"

Background::Background()
{
	m_nebula = new Nebula();
	m_stars = new StarParticleSystem();
	ObjectLoader obj;
	m_moon = new GameEntity(obj.LoadModel("energy.obj"), 
		new Material(2, L"lunarrock_d.png", L"lunarrock_n.png"), 
		new ShaderProgram(L"NormalVertexShader.cso", L"MoonPixelShader.cso"));

	m_moon->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_moon->sendConstantBufferDataToShaderProgram(CBCamera, SPVertex);
	m_moon->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);
	m_moon->sendConstantBufferDataToShaderProgram(CBCamera, SPPixel);

	LightBuffer lightData = {
		XMFLOAT4(0.15, 0.15, 0.15,1),
		XMFLOAT4(1,1,0.5,1),
		XMFLOAT3(3,1,1),
		1,
		XMFLOAT4(1, 1, 1, 0.5),
		XMFLOAT4(0.7,0.7,0.7,1)
	};

	m_moon->lightData = lightData;

	float r = 0.5;
	m_moon->scale(XMFLOAT3(r, r, r));
	m_moon->setPosition(XMFLOAT3(7, 4, 5));
}

Background::~Background()
{
	deleteHelper<Nebula>(m_nebula);
	deleteHelper<StarParticleSystem>(m_stars);
}

void Background::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	m_nebula->draw(viewMatrix, projectionMatrix, camPos, time);
	m_stars->draw(viewMatrix, projectionMatrix, camPos, time);
	m_moon->draw(viewMatrix, projectionMatrix, camPos, time);
}

void Background::update(float dt)
{
	//XMFLOAT4X4 rotMat = m_moon->getRotation();
	m_moon->rotate(XMFLOAT3(0, 0.0025, 0));
}