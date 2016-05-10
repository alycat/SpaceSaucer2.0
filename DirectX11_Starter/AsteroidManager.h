#ifndef _ASTEROIDMANAGER_H
#define _ASTEROIDMANAGER_H
#include "ICollidableEntities.h"
#include "BulletManager.h"

class AsteroidManager : public ICollidableEntities
{
public:
	AsteroidManager(QuadTree& quadTree);
	~AsteroidManager();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
	void update(float dt) override;
	void init(QuadTree collQuadTree, QuadTree healthQuadTree);
	void handleCollision(GameEntity* ge) override;
protected:
	struct Explosion{
		GameEntity* ge;
		float age;
	};

	struct Timer{
		float time;
		bool running;
	};
	std::vector<Explosion> m_explosions;
	std::vector<Timer> m_timer;
	BulletManager* m_bulletMgr;
	Material* m_mat;
	ShaderProgram* m_sp;
	QuadTree m_healthQuadTree;
	QuadTree m_collQuadTree;
};
#endif