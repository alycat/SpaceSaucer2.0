#pragma once
#ifndef _GAME_H
#define _GAME_H
#include <vector>
#include "ObjectLoader.h"
#include "GameEntity.h"
#include "Global.h"
#include "FW1FontWrapper.h"
#include "Player.h"
#include <Audio.h>
#include "include/irrKlang.h"
#include <iostream>
#include "GameTimer.h"
#include "AsteroidManager.h"
#include "CollectableManager.h"
#include "State.h"
#include "QuadTree.h"
#include "RenderTexture.h"
#include "Background.h"
#include "SpriteBatch.h"
#include "SpriteFont.h"
#include "HealthCollectableManager.h"
#include "StateManager.h"
#include "WindowSingleton.h"
#include "Button.h"
#include "HighScoreManager.h"
using namespace DirectX;

#pragma comment(lib, "irrKlang.lib") // link with irrKlang.dll

class Game{
public:
	Game();
	~Game(void);
	State *background;
	void initGame(SamplerState *samplerStates); // sets up the default parameters for the game
	void updateGame(float dt); // main update method for the game

	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time); // Main drawing method for the game
	void drawText(IFW1FontWrapper *pFontWrapper); // handles text rendering
	void reset(); // resets the game to the default state
	void handleCollision(); // handles collisions between the player and an asteroid
	void getHealth(); //for health pickups
	void pickUp(); // for star pickups
	//RenderTexture renderTexture;
protected:
	Background* m_background;
	RenderTexture m_renderTexture;
	AsteroidManager* m_astMgr;
	CollectableManager* m_collMgr;
	HealthCollectableManager* m_health;
	GameEntity* m_renderTarget;
	GameEntity* m_collUI;
	GameEntity* m_pauseGE;
	BulletManager* m_bulletMgr;
	Player* m_player;
	QuadTree quadTree;
	WindowSingleton* wind;
	StateManager* m_stateMgr;
	void DrawUI(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	static DirectX::SpriteBatch *spriteBatch;
	static DirectX::SpriteFont* spriteFont;
	static Timer m_pauseTimer;
	//sound engine for the project
	irrklang::ISoundEngine* engine;
	HighScoreManager *m_highScoreMgr;
	Button *m_pauseButton;
};
#endif