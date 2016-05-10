#ifndef _BACKGROUND_H
#define _BACKGROUND_H

#include "Nebula.h"
#include "StarParticleSystem.h"

class Background{
public:
	Background();
	~Background();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	void update(float dt);
protected:
	Nebula* m_nebula;
	GameEntity* m_moon;
	StarParticleSystem* m_stars;
private:
};
#endif