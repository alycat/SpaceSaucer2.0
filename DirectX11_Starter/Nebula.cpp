#include "Nebula.h"
#include "ShaderProgram.h"
#include "Material.h"
#include "Mesh.h"
#include "Global.h"

Nebula::Nebula()
{
	Vertex init[] = {
		{ { 0, 0, 3 }, { 0, 0, 1 }, { 0, 0 } }, { { 0, 0, 0 } }
	};
	UINT indices[] = { 0 };
	m_ge = new GameEntity(new Mesh(init, indices, 1), 
		nullptr, 
		new ShaderProgram(L"NebulaVertexTexture.cso", L"NebulaPixelTexture.cso", L"NebulaGeometryShader.cso"));

	m_ge->setPosition(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_ge->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
	m_ge->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
	m_ge->transparent = true;
}

Nebula::~Nebula()
{
	deleteHelper<GameEntity>(m_ge);
}

void Nebula::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	m_ge->draw(viewMatrix, projectionMatrix, camPos, time);
}