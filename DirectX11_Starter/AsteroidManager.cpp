#include "AsteroidManager.h"
#include "Global.h"

AsteroidManager::AsteroidManager(QuadTree& quadTree)
{
	LightBuffer lightData = {
		XMFLOAT4(0.75, 0.5, 0.75, 1),
		XMFLOAT4(0.75, 0.5, 0.75, 1),
		XMFLOAT3(0, 0, 1),
		1.0f,
		XMFLOAT4(0, 1, 1, 1),
		XMFLOAT4(1, 1, 1, 1)
	};

	Particle icosahedronParticle[] = {
		{ { 0, 0, 1, 1 }, { 0, 0, 0, 0 } }
	};

	UINT coll_i[] = { 0 };
	D3D11_SO_DECLARATION_ENTRY desc[] =
	{
		{ 0, "SV_POSITION", 0, 0, 4, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};
	ShaderProgram* asteroidShader = new ShaderProgram(L"AsteroidVertexShader.cso", L"AsteroidPixelShader.cso", L"AsteroidGeometryShader.cso", L"IcosahedronEmitter.cso", L"IcosahedronVertexShader.cso", desc, 2);
	Material* asteroidMaterial = new Material(3, L"asteroid.jpg", L"rock.jpg", L"alpha_map.png");
	for (int i = 0; i < NUM_ASTEROIDS; ++i)
	{
		m_objects.push_back(new GameEntity(new Mesh(icosahedronParticle, coll_i, 1),
			asteroidMaterial,
			asteroidShader));
		m_objects[i]->lightData = lightData;
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
		m_objects[i]->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);
		m_objects[i]->scale(XMFLOAT3(0.1f, 0.1f, 0.1f));
		m_timer.push_back(Timer{ 0.0f, false });
		m_objects[i]->objectName = "Asteroid";
		setObjectPosition(m_objects[i]);
		quadTree.insert(m_objects[i]);
	}

	m_mat = new Material(1, L"explosion.png");
	m_sp = new ShaderProgram(L"ExplosionVertexShader.cso", 
		L"ExplosionPixelShader.cso", 
		L"SquareShader.cso", 
		L"ExplosionEmitter.cso", 
		L"IcosahedronVertexShader.cso", desc, 2);
}

void AsteroidManager::init(QuadTree collQuadTree, QuadTree healthQuadTree)
{
	m_collQuadTree = collQuadTree;
	m_healthQuadTree = healthQuadTree;
}

AsteroidManager::~AsteroidManager()
{

}

void AsteroidManager::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	for (int e = 0; e < m_explosions.size(); ++e)
	{
		m_explosions[e].ge->draw(viewMatrix, projectionMatrix, camPos, m_explosions[e].age);
		if (m_stateMgr->gameState == Play)
		{ 
			m_explosions[e].age += 0.025;
			if (m_explosions[e].age >= 2)
			{
				m_explosions.erase(m_explosions.begin() + e, m_explosions.end() + e);
			}
		}
	}
	for (int g = 0; g < m_objects.size(); ++g)
	{
		m_objects[g]->draw(viewMatrix, projectionMatrix, camPos, m_timer[g].time);
	}
}



void AsteroidManager::update(float dt)
{
	for (int i = 0; i < m_objects.size(); ++i)
	{

		m_objects[i]->translate(XMFLOAT3(-10.0f * dt, 0.0f, 0.0f));
		if (m_objects[i]->getPosition()._41 < -wind->windowWidth / 26)
		{
			m_objects[i]->resetPosition();
		}
		if (m_timer[i].running && m_stateMgr->gameState == Play)
		{
			m_timer[i].time += 2;
			if (m_timer[i].time > 100)
			{
				m_timer[i].time = 0;
				m_timer[i].running = false;
				m_objects[i]->resetPosition();
			}
		}
	}
}

void AsteroidManager::handleCollision(GameEntity* ge)
{
	int index = std::find(m_objects.begin(), m_objects.end(), ge) - m_objects.begin();
	if (!m_timer[index].running)
	{
		XMFLOAT4X4 position = ge->getPosition();
		float factor = 2.75;
		Particle particle[] = {
			XMFLOAT4(position._41*3.5, position._42*2.5, position._43, 0.0f),
			XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)
		};
		UINT indices[] = { 0 };
		Explosion explosion = { new GameEntity(new Mesh(particle, indices, 1), m_mat, m_sp), 0.0f };
		explosion.ge->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
		explosion.ge->sendConstantBufferDataToShaderProgram(CBTransform, SPStreamout);
		explosion.ge->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
		explosion.ge->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);
		m_explosions.push_back(explosion);

		m_timer[index].running = true;
	}
}