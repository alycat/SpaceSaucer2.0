#include "Collectable.h"
#include "Game.h"
#include "RenderTexture.h"
#include "Global.h"

//Constructor for Collectable object
Collectable::Collectable(ID3D11SamplerState* samplerState, Mesh* meshReference, Player* playerReference, Game* gameReferencePassed, Material* material){
	// set up the lighting parameters
	wind = WindowSingleton::Instance();
	lighting.ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	lighting.diffuse = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	lighting.lightDirection = XMFLOAT3(0.0f, 0.0f, 1.0f);
	lighting.specular = XMFLOAT4(0.6f, 0.6f, 0.6f, 1.0f);
	lighting.specularPower = 5.0f;

	D3D11_SO_DECLARATION_ENTRY desc[] =
	{
		{ 0, "SV_POSITION", 0, 0, 4, 0 },
		{ 0, "COLOR", 0, 0, 4, 0 }
	};

	sampler = samplerState;
	shaderProgram = new ShaderProgram(L"CollectableVertexShader.cso", L"CollectablePixelShader.cso", L"CollectableGeometryShader.cso");
	ShaderProgram* sparkleShader = new ShaderProgram(L"ExplosionVertexShader.cso", 
		L"SparklePixelShader.cso", 
		L"SparkleGeometryShader.cso", 
		L"ExplosionEmitter.cso", 
		L"IcosahedronVertexShader.cso", desc, 2);
	sparkleMaterial = new Material(1, L"particle.png");
	//collectableMaterial = new Material(device, deviceContext, sampler, L"bullet.jpg", shaderProgram);
	
	collectableMaterial = material;
	//collectableMaterial = new Material(gameReference->renderTexture.GetShaderResourceView(), sampler, shaderProgram);
	player = playerReference;
	mesh = meshReference;

	gameReference = gameReferencePassed;

	// populate the list of collectables, scale them to one tenth the size of the mesh and randomize their position (off the right side of the screen)
	for (int i = 0; i < 1; i++)
	{
		collectables.push_back(new GameEntity(meshReference, collectableMaterial, shaderProgram));
		collectables[i]->scale(XMFLOAT3(0.1f, 0.1f, 0.1f));
		collectables[i]->setPosition(XMFLOAT3(((rand() % 60) + SCREEN_WIDTH/26), ((rand() % 40) - 19.0f), 0.0f));
	}

	collectables[0]->setPosition(XMFLOAT3(0, 5, 0));
}

Collectable::~Collectable(){
	if (sampler){
		delete sampler;
		sampler = nullptr;
	}
	if (collectableMaterial){
		delete collectableMaterial;
		collectableMaterial = nullptr;
	}
	if (shaderProgram){
		delete shaderProgram;
		shaderProgram = nullptr;
	}
}


//Update collectables positions each frame
void Collectable::update(float dt){

	//make bounding boxes
/*	BoundingBox *playerbb = new BoundingBox(XMFLOAT3(player->player->getPosition()._41, player->player->getPosition()._42, player->player->getPosition()._43),
		XMFLOAT3(2.0f, 2.0f, 0.0f));


	//moves collectables across screen (right to left) and respawns them when they leave the screen
	for (unsigned int i = 0; i < 1; i++){
		collectables[i]->translate(XMFLOAT3(-8.0f * dt, 0.0f, 0.0f));


		//._41 is the x value for the position matrix of game entities
		if (collectables[i]->getPosition()._41 < -SCREEN_WIDTH / 26)
			collectables[i]->setPosition(XMFLOAT3(SCREEN_WIDTH/26, (rand() % 40) - 19.0f, 0.0f));}

	// Tests for collisions between the collectables and the player
	for (int i = 0; i < 1; i++){
		BoundingBox *collectablebb = new BoundingBox(XMFLOAT3(collectables[i]->getPosition()._41, collectables[i]->getPosition()._42, collectables[i]->getPosition()._43),
			XMFLOAT3(2.0f, 2.0f, 0.0f));
		//check for intersections
		if (collectablebb->Intersects(*playerbb))
		{
				XMFLOAT4X4 matrix = collectables[i]->getPosition();
				XMFLOAT4 pos = XMFLOAT4(matrix._41*1.15, matrix._42*1.15, matrix._43, 1);
				collectables[i]->setPosition(XMFLOAT3(SCREEN_WIDTH / 26, (rand() % 40) - 19.0f, 0.0f));
				gameReference->pickUp();
				sparkles.push_back(new ExplosionEffect(pos, XMFLOAT4(0, 0, 0, 0), wind->device, wind->deviceContext, sparkleMaterial));
				sparkleTimer.push_back(3);

				//elimate spawning on each other
				for (int g = 0; g < 1; g++)
				{
					BoundingBox *collectablebb2 = new BoundingBox(XMFLOAT3(collectables[g]->getPosition()._41, collectables[g]->getPosition()._42, collectables[g]->getPosition()._43),
						XMFLOAT3(2.0f, 2.0f, 0.0f));
					if (collectablebb2->Intersects(*collectablebb))
					{
						
						collectables[i]->setPosition(XMFLOAT3(30.0f, (rand() % 40) - 19.0f, 0.0f));
					}
				}

		}

		break;
	}
	*/
}



//draw collectables
void Collectable::draw(XMFLOAT4X4 viewMatrix, XMFLOAT4X4 projectionMatrix, XMFLOAT3 camPos){
	UINT offset = 0;
	UINT stride = sizeof(Vertex);

	for (int s = 0; s < sparkles.size(); ++s) {
		if (sparkleTimer[s] > 0) {
			//sparkles[s]->draw(viewMatrix, projectionMatrix, sparkleTimer[s]);
			sparkleTimer[s] -= 0.2;
		}
	}

	for (unsigned int i = 0; i < collectables.size(); i++){

		//UINT offset = 0;
		stride = collectables[i]->mesh->sizeofvertex;
		// Set up the input assembler
		wind->deviceContext->IASetInputLayout(collectables[i]->shaderProgram->inputLayout);
		wind->deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);

		//set values that get passed to matrix constant buffer

		wind->deviceContext->GSSetShader(collectables[i]->shaderProgram->geometryShader, NULL, 0);
		collectables[i]->constantBuffers.cb_transform->m_constantBufferLayout.world = collectables[i]->world;
		collectables[i]->constantBuffers.cb_transform->m_constantBufferLayout.view = viewMatrix;
		collectables[i]->constantBuffers.cb_transform->m_constantBufferLayout.projection = projectionMatrix;

		
		//set values that get passed to lighting constant buffer
		collectables[i]->constantBuffers.cb_light->m_constantBufferLayout.ambient = lighting.ambient;
		collectables[i]->constantBuffers.cb_light->m_constantBufferLayout.diffuse = lighting.diffuse;
		collectables[i]->constantBuffers.cb_light->m_constantBufferLayout.lightDirection = lighting.lightDirection;
		collectables[i]->constantBuffers.cb_light->m_constantBufferLayout.specular = lighting.specular;
		collectables[i]->constantBuffers.cb_light->m_constantBufferLayout.specularPower = lighting.specularPower;

		//set values that get passed to camera constant buffer
		collectables[i]->constantBuffers.cb_camera->m_constantBufferLayout.cameraPosition = XMFLOAT3(SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		collectables[i]->constantBuffers.cb_camera->m_constantBufferLayout.padding = 1.0f;

		//matrix constant buffer
		wind->deviceContext->UpdateSubresource(
			collectables[i]->constantBuffers.cb_transform->constantBuffer,
			0,
			NULL,
			&collectables[i]->constantBuffers.cb_transform->m_constantBufferLayout,
			0,
			0);

		wind->deviceContext->UpdateSubresource(
			collectables[i]->constantBuffers.cb_camera->constantBuffer,
			0,
			NULL,
			&collectables[i]->constantBuffers.cb_camera->m_constantBufferLayout,
			0,
			0);

		wind->deviceContext->UpdateSubresource(
			collectables[i]->constantBuffers.cb_light->constantBuffer,
			0,
			NULL,
			&collectables[i]->constantBuffers.cb_light->m_constantBufferLayout,
			0,
			0);
	
		wind->deviceContext->IASetVertexBuffers(0, 1, &collectables[i]->mesh->v_buffer, &stride, &offset);
		wind->deviceContext->IASetIndexBuffer(collectables[i]->mesh->i_buffer, DXGI_FORMAT_R32_UINT, 0);

		wind->deviceContext->PSSetSamplers(0, 1, &sampler);
		wind->deviceContext->PSSetShaderResources(0, 1, &collectables[i]->material->resources[0]);

		wind->deviceContext->GSSetConstantBuffers(0, 1, &collectables[i]->constantBuffers.cb_transform->constantBuffer);
		// Set the current vertex and pixel shaders, as well the constant buffer for the vert shader
		wind->deviceContext->VSSetShader(collectables[i]->shaderProgram->vertexShader, NULL, 0);
		wind->deviceContext->VSSetConstantBuffers(0, 1, &collectables[i]->constantBuffers.cb_transform->constantBuffer); //set first constant vertex buffer-matrix
		
		wind->deviceContext->PSSetShader(collectables[i]->shaderProgram->pixelShader, NULL, 0);
		wind->deviceContext->PSSetConstantBuffers(0, 1, &collectables[i]->constantBuffers.cb_light->constantBuffer); //set pixel constant buffer-light
		wind->deviceContext->PSSetConstantBuffers(1, 1, &collectables[i]->constantBuffers.cb_camera->constantBuffer); //set second constant vertex buffer-camera

		// Finally do the actual drawing
		wind->deviceContext->DrawIndexed(
			collectables.at(i)->mesh->m_size,	// The number of indices we're using in this draw
			0,
			0);

		wind->deviceContext->GSSetShader(NULL, NULL, 0);
	}
}
