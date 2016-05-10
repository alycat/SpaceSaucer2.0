#include "Player.h"
#include "ObjectLoader.h"

Player* Player::m_instance = nullptr;
GameEntity* Player::m_player = nullptr;
PlayerTrail* Player::trail = nullptr;
Timer Player::m_damageTimer = { 0.0f, false };
Timer Player::m_healthTimer = { 0.0f, false };
int Player::m_health = 10;
int Player::m_stars = 0;
int Player::m_asteroids = 0;
std::string Player::m_name = "PLAYER";

Player* Player::Instance()
{
	if (!m_instance)
	{
		m_instance = new Player();
	}
	return m_instance;
}

int Player::getScore()
{
	return m_stars + m_asteroids;
}

//Constructor for player object
//Params(device, deviceContext, vector of constantbuffers, sampler state, mesh)
Player::Player(){
	LightBuffer lightData = {
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		5.0f,
		XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f),
		XMFLOAT4(0,1,1,1)};

	m_bulletMgr = BulletManager::Instance();

	playerColor = XMFLOAT4(0, 1, 1, 1);
	y_velocity = 0.0f;
	ObjectLoader obj;
	m_player = new GameEntity(obj.LoadModel("pyramid.obj"), 
		new Material(3, L"night.jpg", L"night.jpg", L"Butterfly.png"),
		new ShaderProgram(L"MultiTexVertexShader.cso", L"MultiTexPixelShader.cso"), 4, 2);

	m_player->lightData = lightData;
	m_player->sendConstantBufferDataToShaderProgram(CBTransform, SPVertex);
	m_player->sendConstantBufferDataToShaderProgram(CBCamera, SPVertex);
	m_player->sendConstantBufferDataToShaderProgram(CBLight, SPPixel);

	m_player->translate(XMFLOAT3(0.0f, 0.0f, 1.0f));
	m_player->scale(XMFLOAT3(0.1f, 0.1f, 0.1f));

	trail = new PlayerTrail();
}

void Player::setName(std::string name)
{
	m_name = name;
}

std::string Player::getName()
{
	return m_name;
}

Player::~Player(){
}

void Player::addStar()
{
	m_stars++;
}


int Player::getStarCount()
{
	return m_stars;
}

void Player::addAsteroid()
{
	m_asteroids++;
}

int Player::getAsteroidCount()
{
	return m_asteroids;
}

//return ten times player's health
int Player::getHealth(){
	return m_health;
}

//Decrement Player's health
void Player::takeDamage()
{
	if (!m_damageTimer.isRunning)
	{
		if (m_health > 0)
		{
			m_health--;
			m_damageTimer.isRunning = true;
		}
	}
}

void Player::addHealth()
{
	if (!m_healthTimer.isRunning)
	{
		if (m_health < 10)
		{
			m_health++;
			m_healthTimer.isRunning = true;
		}
	}
}
//Set Player's health ->Percent goes in, player's health goes from 1-10
//Ex: new_health is 100, health will set to 10.
void Player::setHealth(int new_health){
	if (new_health < 0)
		m_health = 0;
	else if (new_health > 10)
		m_health = 10;
	else
		m_health = new_health;
}

void Player::reset()
{
	m_health = 10;
	m_stars = 0;
	m_asteroids = 0;
	m_name = "PLAYER";
	m_player->setPosition(XMFLOAT3(0.0f, 0.0f, 0.0f));
}

void Player::updateTimers()
{
	float inc = 0.1;
	if (m_damageTimer.isRunning)
	{
		m_damageTimer.time+=inc;
		playerColor = XMFLOAT4(min(playerColor.x + 0.025, 1), max(playerColor.y - 0.025, 0), max(playerColor.z - 0.025, 0), 1);
		if (m_damageTimer.time >= 10.0f)
		{
			m_damageTimer.isRunning = false;
			m_damageTimer.time = 0;
		}
	}
	else{
		if (playerColor.x != 0 || playerColor.y != 1 || playerColor.z != 1)
		{
			playerColor = XMFLOAT4(max(playerColor.x - 0.025, 0), min(playerColor.y + 0.025, 1), min(playerColor.z + 0.025, 1), 1);
		}
	}
	if (m_healthTimer.isRunning)
	{
		m_healthTimer.time+=inc;
		if (m_healthTimer.time >= 1.0f)
		{
			m_healthTimer.isRunning = false;
			m_healthTimer.time = 0;
		}
	}
}

//Update player position based on user input
void Player::update(float dt){
	XMFLOAT4X4 position = m_player->getPosition();
	static float t = 0;
	t++;
	float new_y_velocity = cos(t / 50) * 3;
	m_player->translate(XMFLOAT3(0.0f, (new_y_velocity)*dt/1.5, 0.0f));

	if (GetAsyncKeyState('D') & 0x8000){
		m_player->translate(XMFLOAT3(5.0f * dt, 0.0f, 0.0f));
	}
	if (GetAsyncKeyState('A') & 0x8000){
		m_player->translate(XMFLOAT3(-5.0f * dt, 0.0f, 0.0f));
	}
	if (GetAsyncKeyState('W') & 0x8000){
		m_player->translate(XMFLOAT3(0.0f, 5.0f * dt, 0.0f));
		new_y_velocity += 5.0f;
	}
	if (GetAsyncKeyState('S') & 0x8000){
		m_player->translate(XMFLOAT3(0.0f, -5.0f * dt, 0.0f));
		new_y_velocity -= 5.0f;
	}
	if (GetAsyncKeyState('Q') & 0x8000)
	{
		XMFLOAT4X4 position = m_player->getPosition();
		m_bulletMgr->fire(XMFLOAT3(position._41*1.15, position._42 *1.15, 1.0f));
	}
	y_velocity = new_y_velocity;
	m_bulletMgr->update(dt);
	this->updateTimers();
}

//draw player game entity
void Player::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time){
	m_bulletMgr->draw(viewMatrix, projectionMatrix, camPos, time);
	static float prev_y_velocity = 0.0f;
	static float age = 0.0f;
	static float inc = 0.0001;
	if (abs(age) > 0.5){
		inc *= -1;
	}
	age = time - age;
	XMFLOAT4X4 pos = m_player->getPosition();
	XMFLOAT3 position = XMFLOAT3{ pos._41, pos._42, 0 };
	XMFLOAT4 acceleration = XMFLOAT4{ 0, prev_y_velocity, 0, 0 };

	LightBuffer lightData = {
		XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
		XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f),
		XMFLOAT3(0.0f, 0.0f, 1.0f),
		5.0f,
		XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f),
		playerColor };
	m_player->lightData = lightData;
	trail->draw(viewMatrix, projectionMatrix, XMFLOAT3(playerColor.x, playerColor.y, playerColor.z), sin(time)*0.5, position, acceleration);
	m_player->draw(viewMatrix, projectionMatrix, camPos, time);
	prev_y_velocity = y_velocity;
	age += inc;
}

GameEntity* Player::getGameEntity()
{
	return m_player;
}