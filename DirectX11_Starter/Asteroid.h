#pragma once
#ifndef _ASTEROID_H
#define _ASTEROID_H
#include <DirectXMath.h>
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
#include "QuadTree.h"
#include "ConstantBufferContainer.h"

class Game;

using namespace DirectX;

class Asteroid{
public:
	Asteroid() {}
	Asteroid(ID3D11Device* dev, ID3D11DeviceContext* devCtx, ID3D11SamplerState* samplerState, Mesh* meshReference, Player* playerReference, Game* gameReferencePassed, QuadTree* quadTree);
	~Asteroid(void);
	void update(float dt, StateManager *stateManager);
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	GameEntity* getAsteroid();
	// list of asteroids present in the game
	std::vector<GameEntity*> asteroids;
	std::vector<XMFLOAT3> acceleration;
	std::vector<ExplosionEffect*> explosionEffect;
	std::vector<bool> exploding;
	std::vector<float> time;
	std::vector<float> explosionTimer;
private:
	void drawIcosahedron(int index);
	void drawAsteroids(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, int index, float time);
	bool firstPass;
	Player* player;
	Mesh* mesh;
	ShaderProgram* shaderProgram;
	Material* asteroidMaterial;
	Material* explosionMaterial;
	ID3D11SamplerState* sampler;
	ID3D11Device* device;
	ID3D11DeviceContext* deviceContext;
	LightBuffer lighting;
	XMMATRIX asteroidRot;
	QuadTree m_quadTree;

	// reference to the game necesary for handling the non-asteroid results of a player-asteroid collision
	Game* gameReference;

	wchar_t* collision;
	bool notColliding;
	bool canTakeDamage;
};

#endif