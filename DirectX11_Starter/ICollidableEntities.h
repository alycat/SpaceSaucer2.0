#ifndef _ICOLLIDABLEENTITIES_H
#define _ICOLLIDABLEENTITIES_H
#include "GameEntity.h"
#include "WindowSingleton.h"
#include <d3d11.h>
#include <vector>
#include "QuadTree.h"
#include "StateManager.h"
#include "Player.h"

class ICollidableEntities
{
public:
	ICollidableEntities(){}
	virtual ~ICollidableEntities(){};
	
	virtual void handleCollision(GameEntity* ge) = 0;
protected:
	std::vector<GameEntity*> m_objects;
	QuadTree m_quadTree;
	WindowSingleton *wind;
	StateManager* m_stateMgr;
	Player *m_player;

	void testCollision(QuadTree quadTree, GameEntity* ge)
	{
		quadTree.build(&quadTree);
		QuadTree quad = *quadTree.getQuad(ge);
		std::vector<GameEntity*> objects = quad.getObjects();
		for (int i = 0; i < objects.size(); ++i)
		{
			if (ge != objects[i])
				if (ge->boundingBox.Intersects(objects[i]->boundingBox))
					ge->resetPosition();
		}
	}

public:
	virtual void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
	{
		for each(GameEntity* ge in m_objects)
			ge->draw(viewMatrix, projectionMatrix, camPos, time);
	}
	virtual void update(float dt) = 0;

	QuadTree getQuadTree()
	{
		return m_quadTree;
	}

	void addObjectToQuad(GameEntity* ge)
	{
		setObjectPosition(ge);
		m_quadTree.insert(ge);
	}

	void setObjectPosition(GameEntity* ge)
	{
		m_quadTree.build(&m_quadTree);
		bool canSet = false;
		while (!canSet)
		{
			canSet = true;
			ge->resetPosition();
			QuadTree quad = *m_quadTree.getQuad(ge);
			std::vector<GameEntity*> objects = quad.getObjects();
			for each(GameEntity* object in objects)
			{
				if (object->boundingBox.Intersects(ge->boundingBox))
				{
					canSet = false;
					break;
				}
			}
		}
	}
};
#endif