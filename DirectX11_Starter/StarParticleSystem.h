#ifndef _STARPARTICLESYSTEM_H
#define _STARPARTICLESYSTEM_H

#include "Global.h"
#include "GameEntity.h"
#include <DirectXMath.h>
#include <vector>

class StarParticleSystem{
public:
	StarParticleSystem();
	~StarParticleSystem();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
protected:
	std::vector<GameEntity*> m_stars;
private:
};

#endif