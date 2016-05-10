#ifndef NEBULA_H
#define NEBULA_H

#include "Global.h"
#include "GameEntity.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include "ConstantBufferContainer.h"

class Nebula{
public:
	Nebula();
	~Nebula();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
protected:
	GameEntity* m_ge;
	
};
#endif