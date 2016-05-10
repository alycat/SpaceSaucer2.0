#include "Game.h"
#include "WICTextureLoader.h"
#include "SimpleMath.h"

DirectX::SpriteBatch* Game::spriteBatch = nullptr;
DirectX::SpriteFont* Game::spriteFont = nullptr;
Timer Game::m_pauseTimer = { 0.0f, false };

Game::Game()
{
	m_highScoreMgr = HighScoreManager::Instance();
	m_renderTexture.Initialize(wind->windowWidth, wind->windowHeight);
	ObjectLoader obj;
	m_background = new Background();
	m_astMgr = new AsteroidManager(quadTree);
	m_health = new HealthCollectableManager(quadTree);
	m_player = Player::Instance();
	Mesh *mesh = obj.LoadModel("fullScreenQuad.obj");
	m_renderTarget = new GameEntity(mesh,
		new Material(m_renderTexture.GetShaderResourceView()),
		new ShaderProgram(L"FlatVertexShader.cso", L"SolidPixelShader.cso"));

	m_collUI = new GameEntity(mesh,
		new Material(1, L"goldstar.png"),
		new ShaderProgram(L"FlatVertexShader.cso", L"FlatPixelShader.cso"));
	m_collUI->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	//m_collUI->scale(XMFLOAT3(0.1, 0.1, 1));
	m_collUI->setPosition(XMFLOAT3(-wind->windowWidth/35, wind->windowHeight/35, -1.0f));
	m_collUI->scale(XMFLOAT3(0.075, 0.075, 1));
	m_collUI->transparent = true;

	m_pauseGE = new GameEntity(mesh, new Material(m_renderTexture.GetShaderResourceView()), new ShaderProgram(L"PostProcessVertexShader.cso", L"PostProcessPixelShader.cso"));
	m_pauseGE->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_pauseGE->scale(XMFLOAT3(4, 2.5, 1));

	m_renderTarget->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_renderTarget->scale(XMFLOAT3(4,2.5,1));
	m_renderTarget->setPosition(XMFLOAT3(0,0,0));
	m_collMgr = new CollectableManager(quadTree, new Material(m_renderTexture.GetShaderResourceView()));
	if (!spriteBatch)
	{
		spriteBatch = new SpriteBatch(wind->deviceContext);
	}
	if (!spriteFont)
	{
		spriteFont = new SpriteFont(wind->device, L"oblivious16.spritefont");
	}

	m_pauseButton = new Button(L"Back\nto Menu");
	m_pauseButton->setPosition(XMFLOAT3(0, -3, 0));
	m_health->init(m_collMgr->getQuadTree());
	m_astMgr->init(m_collMgr->getQuadTree(), m_health->getQuadTree());
}

Game::~Game(void)
{
	deleteHelper<Background>(m_background);
	deleteHelper<AsteroidManager>(m_astMgr);
}

void Game::initGame(SamplerState *samplerStates)
{
}

// Main update function for the game
void Game::updateGame(float dt)
{
	quadTree.build(&quadTree);
	std::vector<GameEntity *> objects = quadTree.getObjects();
	for each(GameEntity* object in objects)
	{
		QuadTree quad = *quadTree.getQuad(object);
		std::vector<GameEntity*> objs = quad.getObjects();
		for each(GameEntity* obj in objs)
		{
			if (object->boundingBox.Intersects(obj->boundingBox) && object != obj)
			{
				object->resetPosition();
			}
		}
	}
	QuadTree playerQuad = *quadTree.getQuad(m_player->getGameEntity());
	std::vector<GameEntity*> pObjects = playerQuad.getObjects();
	for each(GameEntity* ge in pObjects)
	{
		if (m_player->getGameEntity()->boundingBox.Intersects(ge->boundingBox))
		{
			if (ge->objectName == "Asteroid")
			{
				m_player->takeDamage();
				m_astMgr->handleCollision(ge);
			}
			else if (ge->objectName == "Collectable")
			{
				m_collMgr->handleCollision(ge);
			}
			else if (ge->objectName == "Health")
			{
				m_health->handleCollision(ge);
			}
		}
	}
	if (m_bulletMgr->isActive())
	{
		QuadTree bQuad = *quadTree.getQuad(m_bulletMgr->gameEntity);
		std::vector<GameEntity*> bObjects = bQuad.getObjects();
		for each(GameEntity* bObj in bObjects)
		{
			if (bObj->boundingBox.Intersects(m_bulletMgr->gameEntity->boundingBox))
			{
				if (bObj->objectName == "Asteroid")
				{
					m_player->addAsteroid();
					m_astMgr->handleCollision(bObj);
					m_bulletMgr->handleCollision();
				}
				else{
					m_bulletMgr->handleCollision();
				}
			}
		}
	}

	if (m_pauseButton->getMouseState() == Button::Click)
		m_stateMgr->gameState = Menu;
	if (m_pauseTimer.isRunning)
	{
		m_pauseTimer.time+=0.1f;
		if (m_pauseTimer.time > 1)
		{
			m_pauseTimer.isRunning = false;
			m_pauseTimer.time = 0;
		}
	}
	if (m_stateMgr->gameState == Play)
	{
		m_background->update(dt);
		m_astMgr->update(dt);
		m_collMgr->update(dt);
		m_health->update(dt);
		m_player->update(dt);
	}
	else{
		m_pauseButton->update();
	}

	if (m_player->health <= 0)
	{
		if(m_highScoreMgr->checkScores())
		{
			m_stateMgr->gameState = Win;
		}
		else{
			m_stateMgr->gameState = Lose;
			m_player->reset();
		}
	}
	if (GetAsyncKeyState('P') & 0x8000 && !m_pauseTimer.isRunning)
	{
		m_pauseTimer.isRunning = true;
		if (m_stateMgr->gameState == Play)
			m_stateMgr->gameState = Pause;
		else
			m_stateMgr->gameState = Play;
	}
}

// Handles collisions between the player and an asteroid
void Game::handleCollision()
{
}

// Handles collisions between the player and an HPUp
void Game::getHealth()
{
}

void Game::pickUp()
{
}

// Method where all the actual drawing occurs
void Game::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	float clear[4] = { 0, 0, 0, 0 };
	ID3D11RenderTargetView* rv = wind->renderTargetView;
	m_renderTexture.SetRenderTarget(wind->depthStencilView);
	m_renderTexture.SetBackBufferRenderTarget(wind->depthStencilView);
	m_renderTexture.ClearRenderTarget(wind->depthStencilView, clear);

	m_background->draw(viewMatrix, projectionMatrix, camPos, time);
	m_astMgr->draw(viewMatrix, projectionMatrix, camPos, time);
	m_health->draw(viewMatrix, projectionMatrix, camPos, time);
	m_player->draw(viewMatrix, projectionMatrix, camPos, time);
	(wind->deviceContext)->OMSetRenderTargets(1, &rv, wind->depthStencilView);
	//m_renderTexture.GetBackBufferRenderTarget(wind->depthStencilView);
	m_renderTarget->draw(viewMatrix, projectionMatrix, camPos, time);
	m_collMgr->draw(viewMatrix, projectionMatrix, camPos, time);
	if (m_stateMgr->gameState == Pause)
	{
		m_pauseGE->draw(viewMatrix, projectionMatrix, camPos, time);
	}
	DrawUI(viewMatrix, projectionMatrix, camPos, time);
}

void Game::DrawUI(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	if (m_stateMgr->gameState == Play)
	{
		std::wstring starCount = L"x";
		starCount.append(std::to_wstring(m_player->getStarCount()));

		std::wstring health = std::to_wstring(m_player->health);

		std::wstring asteroid = std::to_wstring(m_player->getAsteroidCount());

		spriteBatch->Begin();
		spriteFont->DrawString(spriteBatch, starCount.c_str(), DirectX::SimpleMath::Vector2(100, 25), Colors::LawnGreen);
		spriteFont->DrawString(spriteBatch, health.c_str(), DirectX::SimpleMath::Vector2(100, 50), Colors::LawnGreen);
		spriteFont->DrawString(spriteBatch, asteroid.c_str(), DirectX::SimpleMath::Vector2(100, 75), Colors::LawnGreen);
		spriteBatch->End();
		m_collUI->draw(viewMatrix, projectionMatrix, camPos, time);
	}
	else{
		wchar_t* pauseString = L"Pause";
		wchar_t* playString = L"Press 'P' to Play";
		XMVECTOR pauseCenter = spriteFont->MeasureString(pauseString)/2;
		XMVECTOR playCenter = spriteFont->MeasureString(playString) / 2;
		spriteBatch->Begin();
		spriteFont->DrawString(spriteBatch, L"Pause", DirectX::SimpleMath::Vector2(wind->windowWidth / 2 - pauseCenter.m128_f32[0], wind->windowHeight / 2 - pauseCenter.m128_f32[1]), Colors::White);
		spriteFont->DrawString(spriteBatch, L"Press 'P' to Play", DirectX::SimpleMath::Vector2(wind->windowWidth / 2 - playCenter.m128_f32[0], wind->windowHeight / 2 + 50), Colors::White);
		spriteBatch->End();
		m_pauseButton->draw(viewMatrix, projectionMatrix, camPos, time);
	}
}

//resets the game after lose condition
void Game::reset()
{
}

