#ifndef _PLAYER_H
#define _PLAYER_H
#include <DirectXMath.h>
#include <vector>
#include "GameEntity.h"
#include "Global.h"
#include "PlayerTrail.h"
#include "BulletManager.h"
#include "GameTimer.h"


class Player{
public:
	static Player* Instance();
	~Player(void);
	void update(float dt);
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	int getHealth(void);
	void setHealth(int new_health);
	void takeDamage();
	void addHealth();
	std::string getName();
	void setName(std::string name);
	int getScore();
	__declspec(property(get = getScore)) int score;
	__declspec(property(get = getHealth, put = setHealth)) int health;
	__declspec(property(get = getName, put = setName)) std::string name;
	GameEntity* getGameEntity();
	void reset();
	void addStar();
	void addAsteroid();
	int getStarCount();
	int getAsteroidCount();
	static GameEntity* m_player;
	XMFLOAT4 playerColor;
	float y_velocity;
protected:
	Player();
	void updateTimers();
	static Player* m_instance;
	static int m_stars;
	static int m_health;
	static int m_asteroids;
	static PlayerTrail* trail;
	BulletManager* m_bulletMgr;
	static Timer m_damageTimer;
	static Timer m_healthTimer;
	static std::string m_name;
};

#endif