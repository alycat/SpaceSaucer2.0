#include "HealthCollectableManager.h"
#include "ObjectLoader.h"

HealthCollectableManager::HealthCollectableManager(QuadTree& quadTree)
{
	LightBuffer lightData = {
		XMFLOAT4(0.25, 0.1, 0, 1),
		XMFLOAT4(1, 0, 0, 1),
		XMFLOAT3(0, 0, 1),
		30,
		XMFLOAT4(1, 1, 1, 1),
		XMFLOAT4(1, 1, 1, 1)
	};

	ObjectLoader obj;
	Mesh* mesh = obj.LoadModel("cross.obj");
	for (int h = 0; h < 2; ++h)
	{
		m_objects.push_back(new GameEntity(mesh, nullptr, new ShaderProgram(L"PhongVertexShader.cso", L"PhongPixelShader.cso")));
		m_objects[h]->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
		m_objects[h]->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);
		m_objects[h]->sendConstantBufferDataToShaderProgram(CBCamera, SPPixel);
		m_objects[h]->lightData = lightData;
		m_objects[h]->scale(XMFLOAT3(0.1, 0.1, 0.1));
		m_objects[h]->objectName = "Health";
		setObjectPosition(m_objects[h]);
		quadTree.insert(m_objects[h]);
	}
}

void HealthCollectableManager::init(QuadTree collQuad)
{
	m_collQuad = collQuad;
}

HealthCollectableManager::~HealthCollectableManager()
{

}

void HealthCollectableManager::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	ICollidableEntities::draw(viewMatrix, projectionMatrix, camPos, time);
}

void HealthCollectableManager::update(float dt)
{
	for each(GameEntity* ge in m_objects)
	{
		ge->translate(XMFLOAT3(-10 * dt, 0, 0));
		if (ge->getPosition()._41 < -wind->windowWidth / 26)
		{
			ge->resetPosition();
		}
	}
}

void HealthCollectableManager::handleCollision(GameEntity* ge)
{
	m_player->addHealth();
	ge->resetPosition();
}