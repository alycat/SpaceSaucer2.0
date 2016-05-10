// ----------------------------------------------------------------------------
//  A few notes on project settings
//
//  - The project is set to use the UNICODE character set
//    - This was changed in Project Properties > Config Properties > General > Character Set
//    - This basically adds a "#define UNICODE" to the project
//
//  - The include directories were automagically correct, since the DirectX 
//    headers and libs are part of the windows SDK
//    - For instance, $(WindowsSDK_IncludePath) is set as a project include 
//      path by default.  That's where the DirectX headers are located.
//
//  - Two libraries had to be manually added to the Linker Input Dependencies
//    - d3d11.lib
//    - d3dcompiler.lib
//    - This was changed in Project Properties > Config Properties > Linker > Input > Additional Dependencies
//
//  - The Working Directory was changed to match the actual .exe's 
//    output directory, since we need to load the compiled shader files at run time
//    - This was changed in Project Properties > Config Properties > Debugging > Working Directory
//
// ----------------------------------------------------------------------------
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define DEBUG

#include <Windows.h>
#include <d3dcompiler.h>
#include "MyDemoGame.h"
#include "WICTextureLoader.h"
#include "WindowSingleton.h"

#include "SplashState.h"
#include "MenuState.h"
#include "TutorialState.h"
#include "LoseState.h"
#include "WinState.h"
#include "ScoreState.h"
#include "StateManager.h"

#pragma region Win32 Entry Point (WinMain)

// Win32 Entry Point
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE prevInstance,
	PSTR cmdLine, int showCmd)
{
	// Enable run-time memory check for debug builds.

#if defined(DEBUG) | defined(_DEBUG)
	//_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

	// Make the game, initialize and run
	MyDemoGame game(hInstance);

	if (!game.Init())
		return 0;

	int toReturn = game.Run();

	//_CrtDumpMemoryLeaks();
	return toReturn;
}

#pragma endregion

#pragma region Constructor / Destructor

MyDemoGame::MyDemoGame(HINSTANCE hInstance) : DirectXGame(hInstance)
{
	// Set up our custom caption and window size
	windowCaption = L"Graphics Programming Project";

	const HWND hDesktop = GetDesktopWindow();
	RECT desktop;
	GetWindowRect(hDesktop, &desktop);

	windowWidth = SCREEN_WIDTH;
	windowHeight = SCREEN_HEIGHT;
}

MyDemoGame::~MyDemoGame()
{
	deleteHelper<Game>(m_game);
	//renderTarget.Shutdown();
	//deleteHelper<GameEntity>(ship);
	ReleaseMacro(deviceContext);
	ReleaseMacro(device);
	ReleaseMacro(depthStencilBuffer);
	ReleaseMacro(depthStencilView);
}

#pragma endregion

#pragma region Initialization

// Initializes the base class (including the window and D3D),
// sets up our geometry and loads the shaders (among other things)
bool MyDemoGame::Init()
{
	if (!DirectXGame::Init())
		return false;

	wind = WindowSingleton::Instance();
	wind->initialize(device, deviceContext, renderTargetView, depthStencilView, windowWidth, windowHeight);
	m_game = new Game();

	ObjectLoader *obj = new ObjectLoader();
	ObjectLoader *obj2 = new ObjectLoader();

	ship = new GameEntity(obj2->LoadModel("pyramid.obj"), new Material(3, L"night.jpg", L"night.jpg", L"Butterfly.png"), new ShaderProgram(L"MultiTexVertexShader.cso", L"MultiTexPixelShader.cso"));
	ship->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	ship->sendConstantBufferDataToShaderProgram(CBCamera, SPVertex);
	ship->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);

	ship->translate(XMFLOAT3(0.0f, 0.0f, 1.0f));
	ship->scale(XMFLOAT3(0.1f, 0.1f, 0.1f));
	//background->setPosition(XMFLOAT3(0, 0, 5));


	// Set up view matrix (camera)
	// In an actual game, update this when the camera moves (every frame)
	XMVECTOR position = XMVectorSet(0, 0, -1, 0);
	XMVECTOR target = XMVectorSet(1, 0, 0, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookAtLH(position, target, up);

	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));

	// Set up world matrix
	// In an actual game, update this when the object moves (every frame)
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W));

	cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
	cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	upDirection = XMVectorSet(0, 1, 0, 0);
	m_stateMgr = StateManager::Instance();
	m_states.push_back(new SplashState());
	m_states.push_back(new MenuState());
	m_states.push_back(new TutorialState());
	m_states.push_back(new LoseState());
	m_states.push_back(new WinState());
	m_states.push_back(new ScoreState());
	timer = new GameTimer();
	return true;
}


#pragma region Window Resizing

// Handles resizing the window and updating our projection matrix to match
void MyDemoGame::OnResize()
{
	// Handle base-level DX resize stuff
	DirectXGame::OnResize();

	wind->windowWidth = windowWidth;
	wind->windowHeight = windowHeight;
	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,
		AspectRatio(),
		0.1f,
		50);
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P));
}
#pragma endregion

#pragma region Game Loop

// Updates the local constant buffer and 
// push it to the buffer on the device
void MyDemoGame::UpdateScene(float dt)
{
	UpdateCamera();
	switch (m_stateMgr->gameState)
	{
	case Splash: {
		timer->Start();
		m_states[0]->update(dt);
		timer->Tick();
	}
		break;
	case Menu: m_states[1]->update(dt);
		break;
	case Tutorial: m_states[2]->update(dt);
		break;
	case Lose: {
		timer->Stop();
		m_states[3]->update(dt);
	}
		break;
	case Win: {
		timer->Stop();
		m_states[4]->update(dt);
	}
		break;
	case Scores: m_states[5]->update(dt);
		break;
	default: {
		timer->Start();
		m_game->updateGame(dt);
		timer->Tick();
	}
	}
}

//Updates our viewMatrix based on the camera's position
void MyDemoGame::UpdateCamera()
{
	// values used to translate and rotate the camera in response to input
	float translationScale = -0.001f;
	float rotationScale = -.01f;
	//Left ad right arrow keys alter X position

	// make all camera manipulations occur at double speed when holding spacebar
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		translationScale *= 2.0f;
		rotationScale *= 2.0f;
	}

	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		gameCam.setDistanceX(translationScale);
		gameCam.setPosition(gameCam.getDistanceX(), gameCam.getDistanceY(), gameCam.getDistanceZ());
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		gameCam.setDistanceX(-translationScale);
		gameCam.setPosition(gameCam.getDistanceX(), gameCam.getDistanceY(), gameCam.getDistanceZ());
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}

	//Up/Down arrow keys alter Y position
	if (GetAsyncKeyState(VK_DOWN) & 0x8000)
	{
		gameCam.setDistanceY(translationScale);
		gameCam.setPosition(gameCam.getDistanceX(), gameCam.getDistanceY(), gameCam.getDistanceZ());
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}
	if (GetAsyncKeyState(VK_UP) & 0x8000)
	{
		gameCam.setDistanceY(-translationScale);
		gameCam.setPosition(gameCam.getDistanceX(), gameCam.getDistanceY(), gameCam.getDistanceZ());
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}

	//5 and 0 on the numpad alter the Z position
	if (GetAsyncKeyState(VK_NUMPAD0) & 0x8000)
	{
		gameCam.setDistanceZ(translationScale);
		gameCam.setPosition(gameCam.getDistanceX(), gameCam.getDistanceY(), gameCam.getDistanceZ());
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}
	if (GetAsyncKeyState(VK_NUMPAD5) & 0x8000)
	{
		gameCam.setDistanceZ(-translationScale);
		gameCam.setPosition(gameCam.getDistanceX(), gameCam.getDistanceY(), gameCam.getDistanceZ());
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}

	//4 and 6 on the numpad will rotate along the X axis
	if (GetAsyncKeyState(VK_NUMPAD4) & 0x8000)
	{
		gameCam.setRotationDistanceX(rotationScale);
		gameCam.setRotation(gameCam.getRotationDistanceX(), gameCam.getRotationDistanceY(), gameCam.getRotationDistanceZ());
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}
	if (GetAsyncKeyState(VK_NUMPAD6) & 0x8000)
	{
		gameCam.setRotationDistanceX(-rotationScale);
		gameCam.setRotation(gameCam.getRotationDistanceX(), gameCam.getRotationDistanceY(), gameCam.getRotationDistanceZ());
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}

	//8 ad 2 on the unmpad will rotate along the y axis
	if (GetAsyncKeyState(VK_NUMPAD8) & 0x8000)
	{
		gameCam.setRotationDistanceY(-rotationScale);
		gameCam.setRotation(gameCam.getRotationDistanceX(), gameCam.getRotationDistanceY(), gameCam.getRotationDistanceZ());
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}
	if (GetAsyncKeyState(VK_NUMPAD2) & 0x8000)
	{
		gameCam.setRotationDistanceY(rotationScale);
		gameCam.setRotation(gameCam.getRotationDistanceX(), gameCam.getRotationDistanceY(), gameCam.getRotationDistanceZ());
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}

	//reset camera back to original position
	if (GetAsyncKeyState('R') & 0x8000)
	{
		gameCam.reset();
		cameraPosition = XMVectorSet(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ(), 0);
		cameraRotation = XMVectorSet(gameCam.getRotationX(), gameCam.getRotationY(), gameCam.getRotationZ(), 0);
	}

	// return the manipulation scales to their normal values
	translationScale = -0.001f;
	rotationScale = -.01f;

	XMMATRIX V = XMMatrixLookToLH(cameraPosition, cameraRotation, upDirection);
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V));


}
// Clear the screen, redraw everything, present
void MyDemoGame::DrawScene()
{
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
	XMFLOAT3 camPos = XMFLOAT3(gameCam.getPositionX(), gameCam.getPositionY(), gameCam.getPositionZ() + 5);
	// Clear the buffer
	deviceContext->ClearRenderTargetView(renderTargetView, color);
	deviceContext->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	float time = timer->TotalTime();
	static float pauseTime = time;
	switch(m_stateMgr->gameState)
	{
	case Splash: m_states[0]->draw(viewMatrix, projectionMatrix, camPos, time);
		break;
	case Menu: m_states[1]->draw(viewMatrix, projectionMatrix, camPos, time);
		break;
	case Tutorial: m_states[2]->draw(viewMatrix, projectionMatrix, camPos, time);
		break;
	case Lose: m_states[3]->draw(viewMatrix, projectionMatrix, camPos, time);
		break;
	case Win: m_states[4]->draw(viewMatrix, projectionMatrix, camPos, time);
		break;
	case Scores: m_states[5]->draw(viewMatrix, projectionMatrix, camPos, time);
		break;
	case Pause:{ 
		m_game->draw(viewMatrix, projectionMatrix, camPos, pauseTime);
		break; 
	}
	default: {
		pauseTime = time;
		m_game->draw(viewMatrix, projectionMatrix, camPos, time);
	}
	}

	// Present the buffer
	HR(swapChain->Present(0, 0));
}

#pragma endregion

