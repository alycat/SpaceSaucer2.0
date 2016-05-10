#include "StarParticleSystem.h"

StarParticleSystem::StarParticleSystem()
{
	Particle particle[] = {
		{ XMFLOAT4(-2.0f, -1.0f, 2.0f, 1.0f), XMFLOAT4(0.0005f, 0.0005f, 0.5f, 0.5f) }
	};
	UINT indices[] = { 0 };

	D3D11_SO_DECLARATION_ENTRY desc[] =
	{
		{ 0, "SV_POSITION", 0, 0, 4, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};

	ShaderProgram* sp = new ShaderProgram(L"GeometryVertexShader.cso", 
		L"GeometryPixelShader.cso", 
		L"GeometryShader.cso", 
		L"GeometryShaderStreamOutput.cso", 
		L"StreamoutVertex.cso", desc, 2);
	
	Material* mat = new Material(1, L"particle.png");

	

	for (unsigned int i = 0; i < 50; ++i)
	{
		GameEntity* ge = new GameEntity(new Mesh(particle, indices, 1), mat, sp);
		m_stars.push_back(ge);
	}

	for each(GameEntity* ge in m_stars)
	{
		ge->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
		ge->sendConstantBufferDataToShaderProgram(CBTransform, SPStreamout);
		ge->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
		//ge->transparent = true;
	}

}

StarParticleSystem::~StarParticleSystem()
{
	for (int i = m_stars.size() - 1; i < -1; --i)
	{
		deleteHelper<GameEntity>(m_stars[i]);
	}
	m_stars.clear();
}


void StarParticleSystem::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	for (unsigned int i = 0; i < m_stars.size(); ++i)
		m_stars[i]->draw(viewMatrix, projectionMatrix, camPos, (cos(time/3) + 1) * 3 + (i * 0.25));
}