#ifndef _COLLECTABLEMANAGER_H
#define _COLLECTABLEMANAGER_H
#include "ICollidableEntities.h"

class CollectableManager : public ICollidableEntities
{
public: 
	CollectableManager(QuadTree& quadTree, Material* material);
	~CollectableManager();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time) override;
	void update(float dt) override;
	void handleCollision(GameEntity* ge) override;
protected:
	struct Sparkle{
		GameEntity* ge;
		float time;
	};
	std::vector<Sparkle> m_sparkles;
	ShaderProgram* m_sp;
	Material* m_mat;
};

#endif