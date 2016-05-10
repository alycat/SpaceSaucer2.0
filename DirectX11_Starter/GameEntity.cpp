#include "GameEntity.h"
#include "SamplerState.h"

GameEntity::GameEntity(Mesh* mesh, Material* mat, float width, float height): g_mesh(mesh), g_mat(mat), 
m_shader(nullptr), m_width(width), m_height(height), initialized(false)/*, wind(WindowSingleton::Instance())*/
{
	m_objectName = "";
	clearTransforms();
	sampler = nullptr;
	samplerState = new SamplerState(sampler);
	samplerState->createSamplerState(wind->device);

	worldMatrix = getWorld();
	initialized = false;
	m_transparent = false;
	for (unsigned int i = 0; i < 5; ++i)
		lastCBtoSPIndices[i] = 0;

	m_lightData.ambient = XMFLOAT4(0, 0, 0, 1);
	m_lightData.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_lightData.highlight = XMFLOAT4(0, 0, 0, 0);
	m_lightData.lightDirection = XMFLOAT3(0, 0, 1);
	m_lightData.specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	m_lightData.specularPower = 5.0f;

	m_particleData.Position = XMFLOAT4(0, 0, 0, 0);
	m_particleData.speed = XMFLOAT4(0, 0, 0, 0);

	m_streamData.age = 0;
	m_streamData.position = XMFLOAT3(0, 0, 0);
	m_streamData.acceleration = XMFLOAT4(0, 0, 0, 0);
}

void GameEntity::resetPosition()
{
	this->setPosition(XMFLOAT3(((rand() % 60) + SCREEN_WIDTH / 26), ((rand() % 40) - 19.0f), 1.0f));
}

const char* GameEntity::getObjectName()
{
	return m_objectName;
}

void GameEntity::setObjectName(const char* objectName)
{
	m_objectName = objectName;
}

GameEntity::GameEntity(Mesh* mesh, Material* mat, ShaderProgram* shader, float width, float height) : g_mesh(mesh), g_mat(mat),
m_shader(shader), m_width(width), m_height(height), initialized(false)
{
	clearTransforms();
	worldMatrix = getWorld();
	sampler = nullptr;
	samplerState = new SamplerState(sampler);
	samplerState->createSamplerState(wind->device);

	initialized = false;

	for (unsigned int i = 0; i < 5; ++i)
		lastCBtoSPIndices[i] = 0;


	m_lightData.ambient = XMFLOAT4(0, 0, 0, 1);
	m_lightData.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_lightData.highlight = XMFLOAT4(0, 0, 0, 0);
	m_lightData.lightDirection = XMFLOAT3(0, 0, 1);
	m_lightData.specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	m_lightData.specularPower = 5.0f;

	m_particleData.Position = XMFLOAT4(0, 0, 0, 0);
	m_particleData.speed = XMFLOAT4(0, 0, 0, 0);

	m_streamData.age = 0;
	m_streamData.position = XMFLOAT3(0, 0, 0);
	m_streamData.acceleration = XMFLOAT4(0, 0, 0, 0);
}

Material* GameEntity::getMaterial(){ return g_mat; }
Mesh* GameEntity::getMesh(){ return g_mesh; }
ShaderProgram* GameEntity::getShaderProgram(){ return m_shader; }
ConstantBufferContainer GameEntity::getConstantBufferContainer(){ return m_container; }

bool GameEntity::isTransparent() { return m_transparent; }
void GameEntity::setTransparency(bool transparent){ m_transparent = transparent; }

void GameEntity::setLightData(LightBuffer lightData){ m_lightData = lightData; }
void GameEntity::setParticleData(Particle particleData){ m_particleData = particleData; }
void GameEntity::setStreamData(StreamoutParticleBuffer streamData){ m_streamData = streamData; }

void GameEntity::sendConstantBufferDataToShaderProgram(ConstantBufferType cb, ShaderProgramType sp)
{
	int index = lastCBtoSPIndices[sp];
	sendConstantBufferDataToShaderProgram(cb, sp, index);
}

void GameEntity::sendConstantBufferDataToShaderProgram(ConstantBufferType cb, ShaderProgramType sp, int index)
{
	lastCBtoSPIndices[sp]++;
	switch (cb)
	{
	case CBTransform:
		sendTransformDataToShaderProgram(sp, index);
		break;
	case CBStreamout:
		sendStreamoutDataToShaderProgram(sp, index);
		break;
	case CBCamera:
		sendCameraDataToShaderProgram(sp, index);
		break;
	case CBParticle:
		sendParticleDataToShaderProgram(sp, index);
		break;
	case CBLight:
		sendLightDataToShaderProgram(sp, index);
		break;
	}
}

void GameEntity::sendTransformDataToShaderProgram(ShaderProgramType sp, int index)
{
	switch (sp)
	{
	case SPVertex:
		this->constantBuffers.cb_transform->sendToVSShader = true;
		this->constantBuffers.cb_transform->indexForVSShader = index;
		break;
	case SPPixel:
		this->constantBuffers.cb_transform->sendToPSShader = true;
		this->constantBuffers.cb_transform->indexForPSShader = index;
		break;
	case SPGeometry:
		this->constantBuffers.cb_transform->sendToGSShader = true;
		this->constantBuffers.cb_transform->indexForGSShader = index;
		break;
	case SPStreamout:
		this->constantBuffers.cb_transform->sendToSOShader = true;
		this->constantBuffers.cb_transform->indexForSOShader = index;
		break;
	case SPInitialVertex:
		this->constantBuffers.cb_transform->sendToIsShader = true;
		this->constantBuffers.cb_transform->indexForISShader = index;
		break;
	}
}
void GameEntity::sendStreamoutDataToShaderProgram(ShaderProgramType sp, int index){
	switch (sp)
	{
	case SPVertex:
		this->constantBuffers.cb_streamParticle->sendToVSShader = true;
		this->constantBuffers.cb_streamParticle->indexForVSShader = index;
		break;
	case SPPixel:
		this->constantBuffers.cb_streamParticle->sendToPSShader = true;
		this->constantBuffers.cb_streamParticle->indexForPSShader = index;
		break;
	case SPGeometry:
		this->constantBuffers.cb_streamParticle->sendToGSShader = true;
		this->constantBuffers.cb_streamParticle->indexForGSShader = index;
		break;
	case SPStreamout:
		this->constantBuffers.cb_streamParticle->sendToSOShader = true;
		this->constantBuffers.cb_streamParticle->indexForSOShader = index;
		break;
	case SPInitialVertex:
		this->constantBuffers.cb_streamParticle->sendToIsShader = true;
		this->constantBuffers.cb_streamParticle->indexForISShader = index;
		break;
	}
}
void GameEntity::sendCameraDataToShaderProgram(ShaderProgramType sp, int index){
	switch (sp)
	{
	case SPVertex:
		this->constantBuffers.cb_camera->sendToVSShader = true;
		this->constantBuffers.cb_camera->indexForVSShader = index;
		break;
	case SPPixel:
		this->constantBuffers.cb_camera->sendToPSShader = true;
		this->constantBuffers.cb_camera->indexForPSShader = index;
		break;
	case SPGeometry:
		this->constantBuffers.cb_camera->sendToGSShader = true;
		this->constantBuffers.cb_camera->indexForGSShader = index;
		break;
	case SPStreamout:
		this->constantBuffers.cb_camera->sendToSOShader = true;
		this->constantBuffers.cb_camera->indexForSOShader = index;
		break;
	case SPInitialVertex:
		this->constantBuffers.cb_camera->sendToIsShader = true;
		this->constantBuffers.cb_camera->indexForISShader = index;
		break;
	}
}
void GameEntity::sendParticleDataToShaderProgram(ShaderProgramType sp, int index){
	switch (sp)
	{
	case SPVertex:
		this->constantBuffers.cb_particle->sendToVSShader = true;
		this->constantBuffers.cb_particle->indexForVSShader = index;
		break;
	case SPPixel:
		this->constantBuffers.cb_particle->sendToPSShader = true;
		this->constantBuffers.cb_particle->indexForPSShader = index;
		break;
	case SPGeometry:
		this->constantBuffers.cb_particle->sendToGSShader = true;
		this->constantBuffers.cb_particle->indexForGSShader = index;
		break;
	case SPStreamout:
		this->constantBuffers.cb_particle->sendToSOShader = true;
		this->constantBuffers.cb_particle->indexForSOShader = index;
		break;
	case SPInitialVertex:
		this->constantBuffers.cb_particle->sendToIsShader = true;
		this->constantBuffers.cb_particle->indexForISShader = index;
		break;
	}
}
void GameEntity::sendLightDataToShaderProgram(ShaderProgramType sp, int index){
	switch (sp)
	{
	case SPVertex:
		this->constantBuffers.cb_light->sendToVSShader = true;
		this->constantBuffers.cb_light->indexForVSShader = index;
		break;
	case SPPixel:
		this->constantBuffers.cb_light->sendToPSShader = true;
		this->constantBuffers.cb_light->indexForPSShader = index;
		break;
	case SPGeometry:
		this->constantBuffers.cb_light->sendToGSShader = true;
		this->constantBuffers.cb_light->indexForGSShader = index;
		break;
	case SPStreamout:
		this->constantBuffers.cb_light->sendToSOShader = true;
		this->constantBuffers.cb_light->indexForSOShader = index;
		break;
	case SPInitialVertex:
		this->constantBuffers.cb_light->sendToIsShader = true;
		this->constantBuffers.cb_light->indexForISShader = index;
		break;
	}
}



GameEntity::~GameEntity(void)
{
	deleteHelper<ShaderProgram>(m_shader);
	deleteHelper<Mesh>(g_mesh);
	deleteHelper<Material>(g_mat);
}

/**
*Reset all matrices to identity matrices
**/
void GameEntity::clearTransforms(void){
	XMMATRIX identity = XMMatrixIdentity();
	XMStoreFloat4x4(&scaleMatrix, identity);
	XMStoreFloat4x4(&rotationMatrix, identity);
	XMStoreFloat4x4(&positionMatrix, identity);
}

/**
* Return position matrix
**/
XMFLOAT4X4 GameEntity::getPosition(void){
	return positionMatrix;
}

XMFLOAT3 GameEntity::getPositionVector(void) {
	return XMFLOAT3(positionMatrix._41, positionMatrix._42, positionMatrix._43);
}

/**
* Return rotation matrix
**/
XMFLOAT4X4 GameEntity::getRotation(void){
	return rotationMatrix;
}

/**
* Return scale matrix
**/
XMFLOAT4X4 GameEntity::getScale(void){
	return scaleMatrix;
}

/**
*Calculate and return world matrix based on current transforms
**/
XMFLOAT4X4 GameEntity::getWorld(void){
	XMMATRIX pos = XMLoadFloat4x4(&positionMatrix);
	XMMATRIX rot = XMLoadFloat4x4(&rotationMatrix);
	XMMATRIX sca = XMLoadFloat4x4(&scaleMatrix);
	XMMATRIX world = pos * rot * sca;
	world = rot * pos * sca;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(world));
	return worldMatrix;
}

/**
*Apply a scale transform to game object
**/
void GameEntity::scale(XMFLOAT3 s){
	XMMATRIX scale = { 
		s.x, 0.0f, 0.0f, 0.0f,
		0.0f, s.y, 0.0f, 0.0f,
		0.0f, 0.0f, s.z, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	XMMATRIX currentScale = XMLoadFloat4x4(&scaleMatrix);
	currentScale *= scale;
	XMStoreFloat4x4(&scaleMatrix, currentScale);
}

/**
*Change position of game object
**/
void GameEntity::translate(XMFLOAT3 t){

	XMMATRIX translate = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		t.x, t.y, t.z, 1.0f };
	XMMATRIX currentTranslation = XMLoadFloat4x4(&positionMatrix);
	currentTranslation *= translate;
	XMStoreFloat4x4(&positionMatrix, currentTranslation);
}

/**
*Apply a rotation to game object
**/
void GameEntity::rotate(XMFLOAT3 r){
	XMMATRIX rotX = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, cos(r.x), sin(r.x), 0.0f,
		0.0f, -sin(r.x), cos(r.x), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f	};

	XMMATRIX rotY = {
		cos(r.y), 0.0f, -sin(r.y), 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		sin(r.y), 0.0f, cos(r.y), 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	XMMATRIX rotZ = {
		cos(r.z), sin(r.z), 0.0f, 0.0f,
		-sin(r.z), cos(r.z), 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };

	XMMATRIX rotation = rotX * rotY * rotZ;
	XMMATRIX currentRotation = XMLoadFloat4x4(&rotationMatrix);
	currentRotation *= rotation;
	XMStoreFloat4x4(&rotationMatrix, currentRotation);
}

void GameEntity::setPosition(XMFLOAT3 pos){
	XMMATRIX position = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		pos.x, pos.y, pos.z, 1.0f
	};

	XMStoreFloat4x4(&positionMatrix, position);
}

BoundingBox GameEntity::getBoundingBox() {
	return BoundingBox(XMFLOAT3(this->positionMatrix._41, this->positionMatrix._42, this->positionMatrix._43), XMFLOAT3(m_width, m_height, 100));
}


void GameEntity::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	ID3D11BlendState* blendState = nullptr;
	D3D11_BLEND_DESC blendDesc;
	blendDesc.AlphaToCoverageEnable = 0;
	blendDesc.IndependentBlendEnable = 0;
	blendDesc.RenderTarget[0].BlendEnable = TRUE;
	blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	HRESULT hr_blend = wind->device->CreateBlendState(&blendDesc, &blendState);

	wind->deviceContext->OMSetBlendState(blendState, NULL, 0xffffffff);

	m_shader->geometryShader ?
		wind->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST) :
		wind->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	if (m_shader->streamoutputShader && !initialized)
	{
		this->drawStreamOut(viewMatrix, projectionMatrix, camPos, time);
		ID3D11Buffer* bufferArray[1] = { 0 };
		wind->deviceContext->SOSetTargets(1, bufferArray, 0);
	}
	this->drawEntity(viewMatrix, projectionMatrix, camPos, time);

	ID3D11RenderTargetView* rv[] = { NULL };
	//wind->deviceContext->OMSetRenderTargets(1, rv, NULL);
}


void GameEntity::drawStreamOut(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	UINT offset[] = { 0 };
	UINT stride = this->g_mesh->sizeofvertex;
	ID3D11Buffer* bufferArray[1] = { 0 };

	wind->deviceContext->IASetInputLayout(m_shader->streamInputLayout);
	wind->deviceContext->IASetVertexBuffers(0, 1, &this->g_mesh->init_buffer, &stride, offset);


	wind->deviceContext->SOSetTargets(1, &this->g_mesh->so_buffer, 0);
	wind->deviceContext->GSSetShader(m_shader->streamoutputShader, NULL, 0);
	wind->deviceContext->VSSetShader(m_shader->initialVertexShader, NULL, 0);

	this->constantBuffers.cb_transform->m_constantBufferLayout.world = this->world;
	this->constantBuffers.cb_transform->m_constantBufferLayout.view = viewMatrix;
	this->constantBuffers.cb_transform->m_constantBufferLayout.projection = projectionMatrix;

	this->constantBuffers.cb_streamParticle->m_constantBufferLayout.age = time;
	this->constantBuffers.cb_streamParticle->m_constantBufferLayout.acceleration = m_streamData.acceleration;
	this->constantBuffers.cb_streamParticle->m_constantBufferLayout.position = m_streamData.position;

	this->constantBuffers.cb_camera->m_constantBufferLayout.cameraPosition = camPos;
	this->constantBuffers.cb_camera->m_constantBufferLayout.padding = 1;

	this->constantBuffers.cb_particle->m_constantBufferLayout.Position = m_particleData.Position;
	this->constantBuffers.cb_particle->m_constantBufferLayout.speed = m_particleData.speed;

	this->constantBuffers.cb_light->m_constantBufferLayout.ambient = m_lightData.ambient;
	this->constantBuffers.cb_light->m_constantBufferLayout.diffuse = m_lightData.diffuse;
	this->constantBuffers.cb_light->m_constantBufferLayout.highlight = m_lightData.highlight;
	this->constantBuffers.cb_light->m_constantBufferLayout.lightDirection = m_lightData.lightDirection;
	this->constantBuffers.cb_light->m_constantBufferLayout.specular = m_lightData.specular;
	this->constantBuffers.cb_light->m_constantBufferLayout.specularPower = m_lightData.specularPower;

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_transform->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_transform->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_streamParticle->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_streamParticle->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_light->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_light->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_camera->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_camera->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_particle->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_particle->m_constantBufferLayout,
		0,
		0);


	if (this->constantBuffers.cb_transform->sendToSOShader){
		int i = this->constantBuffers.cb_transform->indexForSOShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_transform->constantBuffer);
	}
	if (this->constantBuffers.cb_camera->sendToSOShader){
		int i = this->constantBuffers.cb_camera->indexForSOShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_camera->constantBuffer);
	}
	if (this->constantBuffers.cb_light->sendToSOShader){
		int i = this->constantBuffers.cb_light->indexForSOShader;
		wind->deviceContext->GSGetConstantBuffers(i, 1, &this->constantBuffers.cb_light->constantBuffer);
	}
	if (this->constantBuffers.cb_particle->sendToSOShader){
		int i = this->constantBuffers.cb_particle->indexForSOShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_particle->constantBuffer);
	}
	if (this->constantBuffers.cb_streamParticle->sendToSOShader){
		int i = this->constantBuffers.cb_streamParticle->indexForSOShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_streamParticle->constantBuffer);
	}

	if (this->constantBuffers.cb_transform->sendToIsShader){
		int i = this->constantBuffers.cb_transform->indexForISShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_transform->constantBuffer);
	}
	if (this->constantBuffers.cb_camera->sendToIsShader){
		int i = this->constantBuffers.cb_camera->indexForISShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_camera->constantBuffer);
	}
	if (this->constantBuffers.cb_light->sendToIsShader){
		int i = this->constantBuffers.cb_light->indexForISShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_light->constantBuffer);
	}
	if (this->constantBuffers.cb_particle->sendToIsShader){
		int i = this->constantBuffers.cb_particle->indexForISShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_particle->constantBuffer);
	}
	if (this->constantBuffers.cb_streamParticle->sendToIsShader){
		int i = this->constantBuffers.cb_streamParticle->indexForISShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_streamParticle->constantBuffer);
	}

	ID3D11DepthStencilState* depthState = nullptr;
	wind->deviceContext->OMGetDepthStencilState(&depthState, 0);
	wind->deviceContext->OMSetDepthStencilState(NULL, 0);

	wind->deviceContext->IASetIndexBuffer(NULL, DXGI_FORMAT_R32_UINT, 0);
	wind->deviceContext->PSSetShader(NULL, NULL, 0);

	wind->deviceContext->Draw(1, 0);
	initialized = true;
	wind->deviceContext->SOSetTargets(1, bufferArray, 0);
	std::swap(this->g_mesh->v_buffer, this->g_mesh->so_buffer);

	wind->deviceContext->GSSetShader(NULL, NULL, 0);
	wind->deviceContext->OMSetDepthStencilState(depthState, 0);
}

void GameEntity::drawEntity(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time)
{
	UINT offset[] = { 0 };
	UINT stride = this->mesh->sizeofvertex;

	wind->deviceContext->IASetInputLayout(m_shader->inputLayout);
	wind->deviceContext->IASetVertexBuffers(0, 1, &this->g_mesh->v_buffer, &stride, offset);
	wind->deviceContext->IASetIndexBuffer(this->mesh->i_buffer, DXGI_FORMAT_R32_UINT, 0);
	wind->deviceContext->VSSetShader(m_shader->vertexShader, NULL, 0);
	wind->deviceContext->GSSetShader(m_shader->geometryShader, NULL, 0);
	wind->deviceContext->PSSetShader(m_shader->pixelShader, NULL, 0);

	if (this->material)
	{
		wind->deviceContext->PSSetSamplers(0, 1, &samplerState->sampler);
		for (int i = 0; i < this->material->resourceCount; ++i)
		{
			wind->deviceContext->PSSetShaderResources(i, 1, &this->g_mat->resources[i]);
		}
	}

	this->constantBuffers.cb_transform->m_constantBufferLayout.world = this->world;
	this->constantBuffers.cb_transform->m_constantBufferLayout.view = viewMatrix;
	this->constantBuffers.cb_transform->m_constantBufferLayout.projection = projectionMatrix;

	this->constantBuffers.cb_streamParticle->m_constantBufferLayout.age = time;
	this->constantBuffers.cb_streamParticle->m_constantBufferLayout.position = m_streamData.position;
	this->constantBuffers.cb_streamParticle->m_constantBufferLayout.acceleration = m_streamData.acceleration;

	this->constantBuffers.cb_camera->m_constantBufferLayout.cameraPosition = camPos;
	this->constantBuffers.cb_camera->m_constantBufferLayout.padding = 1.0f;

	this->constantBuffers.cb_particle->m_constantBufferLayout.Position = m_particleData.Position;
	this->constantBuffers.cb_particle->m_constantBufferLayout.speed = m_particleData.speed;

	this->constantBuffers.cb_light->m_constantBufferLayout.ambient = m_lightData.ambient;
	this->constantBuffers.cb_light->m_constantBufferLayout.diffuse = m_lightData.diffuse;
	this->constantBuffers.cb_light->m_constantBufferLayout.highlight = m_lightData.highlight;
	this->constantBuffers.cb_light->m_constantBufferLayout.lightDirection = m_lightData.lightDirection;
	this->constantBuffers.cb_light->m_constantBufferLayout.specular = m_lightData.specular;
	this->constantBuffers.cb_light->m_constantBufferLayout.specularPower = m_lightData.specularPower;

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_transform->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_transform->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_streamParticle->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_streamParticle->m_constantBufferLayout,
		0,
		0);
	
	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_light->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_light->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_camera->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_camera->m_constantBufferLayout,
		0,
		0);

	wind->deviceContext->UpdateSubresource(
		this->constantBuffers.cb_particle->constantBuffer,
		0,
		NULL,
		&this->constantBuffers.cb_particle->m_constantBufferLayout,
		0,
		0);

	if (this->constantBuffers.cb_transform->sendToGSShader){
		int i = this->constantBuffers.cb_transform->indexForGSShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_transform->constantBuffer);
	}
	if (this->constantBuffers.cb_light->sendToGSShader){
		int i = this->constantBuffers.cb_light->indexForGSShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_light->constantBuffer);
	}
	if (this->constantBuffers.cb_camera->sendToGSShader){
		int i = this->constantBuffers.cb_camera->indexForGSShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_camera->constantBuffer);
	}
	if (this->constantBuffers.cb_particle->sendToGSShader){
		int i = this->constantBuffers.cb_particle->indexForGSShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_particle->constantBuffer);
	}
	if (this->constantBuffers.cb_streamParticle->sendToGSShader){
		int i = this->constantBuffers.cb_streamParticle->indexForGSShader;
		wind->deviceContext->GSSetConstantBuffers(i, 1, &this->constantBuffers.cb_streamParticle->constantBuffer);
	}

	if (this->constantBuffers.cb_transform->sendToVSShader){
		int i = this->constantBuffers.cb_transform->indexForVSShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_transform->constantBuffer);
	}
	if (this->constantBuffers.cb_light->sendToVSShader){
		int i = this->constantBuffers.cb_light->indexForVSShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_light->constantBuffer);
	}
	if (this->constantBuffers.cb_camera->sendToVSShader){
		int i = this->constantBuffers.cb_camera->indexForVSShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_camera->constantBuffer);
	}
	if (this->constantBuffers.cb_particle->sendToVSShader){
		int i = this->constantBuffers.cb_particle->indexForVSShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_particle->constantBuffer);
	}
	if (this->constantBuffers.cb_streamParticle->sendToVSShader){
		int i = this->constantBuffers.cb_streamParticle->indexForVSShader;
		wind->deviceContext->VSSetConstantBuffers(i, 1, &this->constantBuffers.cb_streamParticle->constantBuffer);
	}

	if (this->constantBuffers.cb_transform->sendToPSShader){
		int i = this->constantBuffers.cb_transform->indexForPSShader;
		wind->deviceContext->PSSetConstantBuffers(i, 1, &this->constantBuffers.cb_transform->constantBuffer);
	}
	if (this->constantBuffers.cb_light->sendToPSShader){
		int i = this->constantBuffers.cb_light->indexForPSShader;
		wind->deviceContext->PSSetConstantBuffers(i, 1, &this->constantBuffers.cb_light->constantBuffer);
	}
	if (this->constantBuffers.cb_camera->sendToPSShader){
		int i = this->constantBuffers.cb_camera->indexForPSShader;
		wind->deviceContext->PSSetConstantBuffers(i, 1, &this->constantBuffers.cb_camera->constantBuffer);
	}
	if (this->constantBuffers.cb_particle->sendToPSShader){
		int i = this->constantBuffers.cb_particle->indexForPSShader;
		wind->deviceContext->PSSetConstantBuffers(i, 1, &this->constantBuffers.cb_particle->constantBuffer);
	}
	if (this->constantBuffers.cb_streamParticle->sendToPSShader){
		int i = this->constantBuffers.cb_streamParticle->indexForPSShader;
		wind->deviceContext->PSSetConstantBuffers(i, 1, &this->constantBuffers.cb_streamParticle->constantBuffer);
	}
	
	if (m_shader->streamoutputShader)
		wind->deviceContext->DrawAuto();
	else
		wind->deviceContext->DrawIndexed(this->g_mesh->m_size, 0, 0);

	wind->deviceContext->GSSetShader(NULL, NULL, 0);
}