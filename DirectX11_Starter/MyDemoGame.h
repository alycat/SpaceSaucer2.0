#pragma once

#include <DirectXMath.h>
#include <vector>
#include "DirectXGame.h"
#include "Global.h"
#include "GameEntity.h"
#include "SpriteFont.h"
#include "Camera.h"
#include "FW1FontWrapper.h"
#include "ObjectLoader.h"
#include <iostream>
#include <string>
#include <cstdio>
#include <ctime>
#include "Game.h"
#include "GameTimer.h"
#include "RenderTexture.h"
#include "ConstantBufferContainer.h"
#include "State.h"
//#include "include/irrKlang.h"

// Include run-time memory checking in debug builds
#if defined(DEBUG) || defined(_DEBUG)
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif

// For DirectX Math
using namespace DirectX;


// Vertex struct for triangles
/*struct Vertex
{
XMFLOAT3 Position;
XMFLOAT4 Color;
};*/

// Struct to match vertex shader's constant buffer
// You update one of these locally, then push it to the corresponding
// constant buffer on the device when it needs to be updated
struct VertexShaderConstantBufferLayout
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};


// Demo class which extends the base DirectXGame class
class MyDemoGame : public DirectXGame
{
public:
	MyDemoGame(HINSTANCE hInstance);
	~MyDemoGame();

	// Overrides for base level methods
	bool Init();
	void OnResize();
	void UpdateScene(float dt);
	void DrawScene();
	void PostProcessDraw();
	void UpdateCamera();

	XMFLOAT3 XMFLOAT3Cross(XMFLOAT3 a, XMFLOAT3 b);
	BOOL uiInitialized = false;

protected:
	// Initialization for our "game" demo
	void CreateGeometryBuffers();
	void LoadShadersAndInputLayout();
	void HandleUIClick(int x, int y);
	Game* m_game;
	RenderTexture* renderTarget;
	WindowSingleton *wind;
	GameEntity *ship;
	GameTimer *timer;

	// The matrices to go from model space
	// to screen space
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 viewMatrix;
	XMFLOAT4X4 projectionMatrix;

	Camera gameCam;
	XMVECTOR cameraPosition;
	XMVECTOR cameraRotation;
	XMVECTOR upDirection;
	
	std::vector<State*> m_states;
	StateManager* m_stateMgr;
	static bool init;
};
