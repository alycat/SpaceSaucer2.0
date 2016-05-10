#ifndef _HEALTHCOLLECTABLEMANAGER_H
#define _HEALTHCOLLECTABLEMANAGER_H
#include "ICollidableEntities.h"

class HealthCollectableManager: public ICollidableEntities{
public:
	HealthCollectableManager(QuadTree& quadTree);
	~HealthCollectableManager();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
	void update(float dt) override;
	void init(QuadTree collQuad);
	void handleCollision(GameEntity* ge) override;
protected:
	QuadTree m_collQuad;
};
#endif