#pragma once
#ifndef _Collectable_H
#define _Collectable_H
//#include "Game.h"
//#include <DirectXMath.h>
#include <vector>
#include "ConstantBuffer.h"
#include "ObjectLoader.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Material.h"
#include "ShaderProgram.h"
#include "SamplerState.h"
#include "Global.h"
#include "Camera.h"
#include "StateManager.h"
#include "FW1FontWrapper.h"
#include "Player.h"
#include "StateManager.h"
#include "SimpleMath.h"
#include "ExplosionEffect.h"
#include "ConstantBufferContainer.h"
#include "WindowSingleton.h"
class Game;

using namespace DirectX;

class Collectable{
public:
	Collectable(ID3D11SamplerState* samplerState, Mesh* meshReference, Player* playerReference, Game* gameReferencePassed, Material* material = nullptr);
	~Collectable(void);
	void update(float dt);
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos);
	GameEntity* getCollectable();

	// list of Collectables present in the game
	std::vector<GameEntity*> collectables;
	std::vector<ExplosionEffect*> sparkles;
	std::vector<float> sparkleTimer;
private:
	Player* player;
	Mesh* mesh;
	//RenderTextureClass renderTexture;
	ShaderProgram* shaderProgram;
	Material* collectableMaterial;
	Material* sparkleMaterial;
	ID3D11SamplerState* sampler;
	LightBuffer lighting;

	// reference to the gam
	Game* gameReference;
	wchar_t* collision;
	WindowSingleton* wind;
};

#endif