#include "PlayerTrail.h"

PlayerTrail::PlayerTrail(){
	
	Vertex v[] = { 
		{
			{ 0, 0, 1 },
			{ 0, 0, 1 },
			{ 0, 0 },
			{ 0, 0, 0 } 
		}
	};

	UINT indices[1] = { 0 };
	Mesh *trail = new Mesh(v, indices, 1);
	object = new GameEntity(trail, 
		nullptr,
		new ShaderProgram(L"TrailVertexShader.cso", L"TrailPixelShader.cso", L"TrailGeometryShader.cso"));

	object->sendConstantBufferDataToShaderProgram(CBStreamout, SPVertex);
	object->sendConstantBufferDataToShaderProgram(CBTransform, SPGeometry);
	object->sendConstantBufferDataToShaderProgram(CBCamera, SPGeometry);
}

PlayerTrail::~PlayerTrail(){

}

void PlayerTrail::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos, float time, XMFLOAT3 position, XMFLOAT4 acceleration){
	StreamoutParticleBuffer streamData = {
		0,
		position,
		acceleration
	};
	object->streamData = streamData;
	object->draw(viewMatrix, projectionMatrix, camPos, time);
}