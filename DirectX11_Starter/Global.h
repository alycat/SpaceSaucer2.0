#ifndef _GLOBAL_H
#define _GLOBAL_H
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }
#define new new(1, __FILE__, __LINE__)
#include <DirectXMath.h>
#define NUM_ASTEROIDS 7
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#include "SimpleMath.h"
#include <memory>
using namespace DirectX;

enum GameState{
	Splash = 0,
	Menu,
	Tutorial,
	Pause,
	Play,
	Win,
	Lose,
	Credits,
	Scores,
	NUM_STATES
};

struct Timer{
	float time;
	bool isRunning;
};


struct Vertex{
	XMFLOAT3 Position;
	XMFLOAT3 Normal;
	XMFLOAT2 UVs;
	XMFLOAT3 Tangent;
};

struct Triangle{
	int index[3];
};

struct Phong{
	XMFLOAT3 Position;
	XMFLOAT4 Color;
	XMFLOAT2 uv;
	XMFLOAT3 Normal;
	XMFLOAT3 lightPos;
};

//Vector struct for our camera class
struct Vector
{
	float x;
	float y;
	float z;
};

struct TransformationBuffer
{
	XMFLOAT4X4 world;
	XMFLOAT4X4 view;
	XMFLOAT4X4 projection;
};

struct StreamoutParticleBuffer
{
	float age;
	XMFLOAT3 position;
	XMFLOAT4 acceleration;
};

//Camera Constant Buffer Data Layout
struct CameraBuffer
{
	XMFLOAT3 cameraPosition;
	float padding;
};

//Light Constant Buffer Data Layout
struct LightBuffer
{
	XMFLOAT4 ambient;
	XMFLOAT4 diffuse;
	XMFLOAT3 lightDirection;
	float specularPower;
	XMFLOAT4 specular;
	XMFLOAT4 highlight;
};

struct Particle{
	XMFLOAT4 Position;
	XMFLOAT4 speed;
};

template <class T>
void deleteHelper(T* ptr)
{
	if (ptr)
		delete ptr;
	ptr = nullptr;
}

struct ShaderToCBuffer
{
	bool send;
	int index;
};

enum ConstantBufferType{
	CBTransform = 0,
	CBStreamout,
	CBParticle,
	CBCamera,
	CBLight
};

enum ShaderProgramType{
	SPVertex = 0,
	SPPixel,
	SPGeometry,
	SPStreamout,
	SPInitialVertex
};

#endif