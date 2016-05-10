#ifndef PLAYERTRAIL_H
#define PLAYERTRAIL_H

#include "Global.h"
#include "GameEntity.h"
#include "Material.h"
#include "ConstantBuffer.h"
#include "Mesh.h"
#include <Windows.h>
#include <d3d11.h>
#include <vector>
#include "ConstantBufferContainer.h"

class PlayerTrail{
public:
	PlayerTrail();
	~PlayerTrail();
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time, XMFLOAT3 position, XMFLOAT4 acceleration);
	GameEntity *object;
};
#endif