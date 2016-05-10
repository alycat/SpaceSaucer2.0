#ifndef _GAMEENTITY_H
#define _GAMEENTITY_H

#include "Mesh.h"
#include "Material.h"
#include "ConstantBufferContainer.h"
#include "WindowSingleton.h"
#include "Global.h"
#include "SamplerState.h"

class GameEntity{
public:
	Mesh* g_mesh;
	Material* g_mat;
protected:
	XMFLOAT4X4 worldMatrix;
	XMFLOAT4X4 rotationMatrix;
	XMFLOAT4X4 positionMatrix;
	XMFLOAT4X4 scaleMatrix;
	float m_height;
	float m_width;
	ShaderProgram* m_shader;
	ConstantBufferContainer m_container;
	void drawStreamOut(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	void drawEntity(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);
	bool initialized;
	bool m_transparent;
	bool m_renderTargetTexture;
	SamplerState *samplerState;
	LightBuffer m_lightData;
	Particle m_particleData;
	StreamoutParticleBuffer m_streamData;
	const char* m_objectName;
public:
	GameEntity(Mesh* mesh, Material* mat, float width = 1, float height = 1);
	GameEntity(Mesh* mesh, Material* mat, ShaderProgram* shader, float width = 1.0f, float height = 1.0f);
	~GameEntity(void);
	void clearTransforms(void);
	void draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time);

	//getters
	XMFLOAT4X4 getWorld(void);
	XMFLOAT4X4 getRotation(void);
	XMFLOAT4X4 getPosition(void);
	XMFLOAT3 getPositionVector(void);
    XMFLOAT4X4 getScale(void);
	ShaderProgram* getShaderProgram();
	Material* getMaterial();
	Mesh* getMesh();
	BoundingBox getBoundingBox();
	bool isTransparent();
	void setTransparency(bool transparent);
	void setLightData(LightBuffer lightData);
	void setParticleData(Particle particleData);
	void setStreamData(StreamoutParticleBuffer streamData);
	void resetPosition();
	const char* getObjectName();
	void setObjectName(const char* objectName);

	ConstantBufferContainer getConstantBufferContainer();
	WindowSingleton* wind;

	
	ID3D11SamplerState* sampler;

	//setters
	void setScale(XMFLOAT4X4 scale);
	void scale(XMFLOAT3 scale);
	void translate(XMFLOAT3 translate);
	void rotate(XMFLOAT3 rotate);
	void setPosition(XMFLOAT3 pos);

	void sendConstantBufferDataToShaderProgram(ConstantBufferType cb, ShaderProgramType sp);
	void sendConstantBufferDataToShaderProgram(ConstantBufferType cb, ShaderProgramType sp, int index);

	//read-only properties
	__declspec(property(get = getShaderProgram)) ShaderProgram* shaderProgram;
	__declspec(property(get = getMaterial)) Material* material;
	__declspec(property(get = getMesh)) Mesh* mesh;
	__declspec(property(get = getWorld)) XMFLOAT4X4 world;
	__declspec(property(get = getRotation)) XMFLOAT4X4 rotation;
	__declspec(property(get = getScale)) XMFLOAT4X4 scalar;
	__declspec(property(get = getBoundingBox)) BoundingBox boundingBox;
	__declspec(property(get = getConstantBufferContainer)) ConstantBufferContainer constantBuffers;
	__declspec(property(get = isTransparent, put = setTransparency)) bool transparent;
	__declspec(property(put = setLightData)) LightBuffer lightData;
	__declspec(property(put = setParticleData)) Particle particleData;
	__declspec(property(put = setStreamData)) StreamoutParticleBuffer streamData;
	__declspec(property(get = getObjectName, put = setObjectName)) const char* objectName;
protected:
	int lastCBtoSPIndices[5];

	void sendTransformDataToShaderProgram(ShaderProgramType sp, int index);
	void sendStreamoutDataToShaderProgram(ShaderProgramType sp, int index);
	void sendCameraDataToShaderProgram(ShaderProgramType sp, int index);
	void sendParticleDataToShaderProgram(ShaderProgramType sp, int index);
	void sendLightDataToShaderProgram(ShaderProgramType sp, int index);
};
#endif