#ifndef _BULLETMANAGER_H
#define _BULLETMANAGER_H

#include "Global.h"
#include "WindowSingleton.h"
#include "GameEntity.h"

class BulletManager{
public:
	static BulletManager* Instance();
	static GameEntity* getGameEntity();
	XMFLOAT3 getPosition();
	void setPosition(XMFLOAT3 position);
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float age);
	void handleCollision();
	void update(float dt);
	bool isActive();
	void fire(XMFLOAT3 playerPosition);
	__declspec(property(get = getPosition, put = setPosition)) XMFLOAT3 position;
	__declspec(property(get = getGameEntity)) GameEntity* gameEntity;
protected:
	BulletManager();
	static BulletManager* m_instance;
	static GameEntity* m_bullet;
	static float m_age;
	static bool canShoot;
	WindowSingleton* wind;
private:
};

#endif