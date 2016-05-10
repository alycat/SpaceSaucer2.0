#ifndef _CONSTANTBUFFER_H
#define _CONSTANTBUFFER_H
#include <d3d11.h>
#include <DirectXMath.h>
#include <unordered_map>
#include <string>
#include "Global.h"
#include "WindowSingleton.h"

using namespace DirectX;
template <class T>
class ConstantBuffer{
private:
	WindowSingleton* wind;
	ShaderToCBuffer dataToShader[5];
public:
	int c_byteWidth;
	T m_constantBufferLayout;
	ID3D11Buffer* constantBuffer;
	ConstantBuffer(T buffer, 
		ShaderToCBuffer vsShader = { false, 0 }, 
		ShaderToCBuffer psShader = { false, 0 }, 
		ShaderToCBuffer gsShader = { false, 0 }, 
		ShaderToCBuffer isShader = { false, 0 }, 
		ShaderToCBuffer soShader = { false, 0 })
	{
		dataToShader[0] = vsShader;
		dataToShader[1] = psShader;
		dataToShader[2] = gsShader;
		dataToShader[3] = isShader;
		dataToShader[4] = soShader;
		wind = WindowSingleton::Instance();
		c_byteWidth = sizeof(T);
		setUpConstantBuffer();
	}
	~ConstantBuffer(void);

	bool isSentToVSShader(){ return dataToShader[0].send; }
	bool isSentToPSShader(){ return dataToShader[1].send; }
	bool isSentToGSShader(){ return dataToShader[2].send; }
	bool isSentToISShader(){ return dataToShader[3].send; }
	bool isSentToSOShader(){ return dataToShader[4].send; }

	void setCanSendToVSShader(bool canSend){ dataToShader[0].send = canSend; }
	void setCanSendToPSShader(bool canSend){ dataToShader[1].send = canSend; }
	void setCanSendToGSShader(bool canSend){ dataToShader[2].send = canSend; }
	void setCanSendToISShader(bool canSend){ dataToShader[3].send = canSend; }
	void setCanSendToSOShader(bool canSend){ dataToShader[4].send = canSend; }

	int getIndexForVSShader(){ return dataToShader[0].index; }
	int getIndexForPSShader(){ return dataToShader[1].index; }
	int getIndexForGSShader(){ return dataToShader[2].index; }
	int getIndexForISShader(){ return dataToShader[3].index; }
	int getIndexForSOShader(){ return dataToShader[4].index; }

	void setIndexForVSShader(int index){ dataToShader[0].index = index; }
	void setIndexForPSShader(int index){ dataToShader[1].index = index; }
	void setIndexForGSShader(int index){ dataToShader[2].index = index; }
	void setIndexForISShader(int index){ dataToShader[3].index = index; }
	void setIndexForSOShader(int index){ dataToShader[4].index = index; }

	__declspec(property(get = isSentToVSShader, put = setCanSendToVSShader)) bool sendToVSShader;
	__declspec(property(get = isSentToPSShader, put = setCanSendToPSShader)) bool sendToPSShader;
	__declspec(property(get = isSentToGSShader, put = setCanSendToGSShader)) bool sendToGSShader;
	__declspec(property(get = isSentToISShader, put = setCanSendToISShader)) bool sendToIsShader;
	__declspec(property(get = isSentToSOShader, put = setCanSendToSOShader)) bool sendToSOShader;

	__declspec(property(get = getIndexForVSShader, put = setIndexForVSShader)) int indexForVSShader;
	__declspec(property(get = getIndexForPSShader, put = setIndexForPSShader)) int indexForPSShader;
	__declspec(property(get = getIndexForGSShader, put = setIndexForGSShader)) int indexForGSShader;
	__declspec(property(get = getIndexForISShader, put = setIndexForISShader)) int indexForISShader;
	__declspec(property(get = getIndexForSOShader, put = setIndexForSOShader)) int indexForSOShader;

	
private:
	void setUpConstantBuffer(){
		D3D11_BUFFER_DESC cBufferDesc;
		cBufferDesc.ByteWidth = c_byteWidth;
		cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
		cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		cBufferDesc.CPUAccessFlags = 0;
		cBufferDesc.MiscFlags = 0;
		cBufferDesc.StructureByteStride = 0;
		wind->device->CreateBuffer(
			&cBufferDesc,
			NULL,
			&constantBuffer);
	}

};
#endif