#include "BulletManager.h"

BulletManager* BulletManager::m_instance = nullptr;
GameEntity* BulletManager::m_bullet = nullptr;
bool BulletManager::canShoot = true;
float BulletManager::m_age = 0.0f;

BulletManager::BulletManager()
{
	LightBuffer lightData = {
		XMFLOAT4(0, 0, 0, 1),
		XMFLOAT4(0.5, 0.5, 0.5, 1.0f),
		XMFLOAT3(0, 0, 1),
		5.0,
		XMFLOAT4(0.6, 0.6, 0.6, 1),
		XMFLOAT4(0,0,0,0)
	};

	Vertex vertex[] = {
		{ { 0, 0, 1 }, { 0, 0, 1 }, { 0, 0 }, { 0, 0, 0 } },
		{ { 0, 0.125, 1 }, { 0, 0, 1 }, { 0, 0 }, { 0, 0, 0 } },
		{ { 0, 0.25, 1 }, { 0, 0, 1 }, { 0, 0 }, { 0, 0, 0 } }
	};

	UINT indices[] = { 0, 1, 2 };

	m_bullet = new GameEntity(new Mesh(vertex, indices, 3),
		new Material(1, L"bullet.png"),
		new ShaderProgram(L"BulletVertexShader.cso", L"BulletPixelShader.cso", L"BulletGeometryShader.cso"), 0.5, 0.5);

	m_bullet->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_bullet->sendConstantBufferDataToShaderProgram(CBStreamout, SPGeometry);
	m_bullet->lightData = lightData;
	m_bullet->scale(XMFLOAT3(0.1, 0.1, 0.1));
}

BulletManager* BulletManager::Instance()
{
	if (!m_instance)
	{
		m_instance = new BulletManager();
	}
	return m_instance;
}

GameEntity* BulletManager::getGameEntity()
{
	return m_bullet;
}

XMFLOAT3 BulletManager::getPosition()
{
	return m_bullet->getPositionVector();
}

void BulletManager::setPosition(XMFLOAT3 position)
{
	m_bullet->setPosition(position);
}

void BulletManager::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float age)
{
	if (!canShoot)
		m_bullet->draw(viewMatrix, projectionMatrix, camPos, m_age);
}

bool BulletManager::isActive()
{
	return !canShoot;
}

void BulletManager::update(float dt)
{
	m_age = dt * 100;

	if (!canShoot)
		m_bullet->translate(XMFLOAT3(15 * dt, sin(dt*10)/10, 0.0f));

	if (m_bullet->getPosition()._41 > (wind->windowWidth) / 26.0f)
	{
		if (!canShoot)
			canShoot = true;
	}
}

void BulletManager::fire(XMFLOAT3 playerPosition)
{
	if (canShoot)
	{
		canShoot = false;
		m_bullet->setPosition(playerPosition);
	}
}

void BulletManager::handleCollision()
{
	if (!canShoot)
		canShoot = true;
}