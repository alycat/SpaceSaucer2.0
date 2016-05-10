#include "CollectableManager.h"
#include "DirectXGame.h"

CollectableManager::CollectableManager(QuadTree& quadTree, Material* material)
{

	m_player = Player::Instance();
	Vertex init[] = {
		{ { 0, 0, 0 }, { 0, 1, 0 }, { 0, 0 } }, { { 0, 0, 0 } }
	};

	UINT indices[1] = { 0 };

	for (int i = 0; i < 3; ++i)
	{
		m_objects.push_back(new GameEntity(new Mesh(init, indices, 1), 
			material,
			new ShaderProgram(L"CollectableVertexShader.cso", L"CollectablePixelShader.cso", L"CollectableGeometryShader.cso"), 2, 2));

		m_objects[i]->scale(XMFLOAT3(0.1, 0.1, 0.1));
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBTransform, SPPixel);
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBCamera, SPPixel);
		m_objects[i]->objectName = "Collectable";
		setObjectPosition(m_objects[i]);
		quadTree.insert(m_objects[i]);
	}

	D3D11_SO_DECLARATION_ENTRY desc[] =
	{
		{ 0, "SV_POSITION", 0, 0, 4, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};

	m_sp = new ShaderProgram(L"ExplosionVertexShader.cso",
		L"SparklePixelShader.cso",
		L"SparkleGeometryShader.cso",
		L"ExplosionEmitter.cso",
		L"IcosahedronVertexShader.cso", desc, 2);

	m_mat = new Material(1, L"particle.png");


}

CollectableManager::~CollectableManager()
{

}


void CollectableManager::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectMatrix, XMFLOAT3 camPos, float time)
{
	for (int s = 0; s < m_sparkles.size(); ++s)
	{
		float t = m_sparkles[s].time;
		if (t > 0)
		{
			m_sparkles[s].ge->draw(viewMatrix, projectMatrix, camPos, t);
			if (m_stateMgr->gameState == Play)
				m_sparkles[s].time -= 0.2;
		}
		else
		{
			m_sparkles.erase(m_sparkles.begin() + s, m_sparkles.begin() + s);
		}
	}
	ICollidableEntities::draw(viewMatrix, projectMatrix, XMFLOAT3(wind->windowWidth, wind->windowHeight, 0), time);
}

void CollectableManager::update(float dt)
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

void CollectableManager::handleCollision(GameEntity* ge)
{
	int index = std::find(m_objects.begin(), m_objects.end(), ge) - m_objects.begin();

	XMFLOAT4X4 position = ge->getPosition();

	Particle particle[] = {{ XMFLOAT4(position._41 * 1.15f, position._42 * 1.15f, position._43, 1), XMFLOAT4(0,0,0,0) }};
	UINT indices[1] = { 0 };

	Sparkle sparkle = { new GameEntity(new Mesh(particle, indices, 1), m_mat, m_sp), 3.0f };

	sparkle.ge->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
	sparkle.ge->sendConstantBufferDataToShaderProgram(CBTransform, SPStreamout);
	sparkle.ge->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
	m_sparkles.push_back(sparkle);
	m_player->addStar();
	ge->resetPosition();
}