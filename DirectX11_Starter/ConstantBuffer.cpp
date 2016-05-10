/*#include "ConstantBuffer.h"
#include "Global.h"

template <typename T>
ConstantBuffer::ConstantBuffer(T buffer, ID3D11Device* device)
{
	c_byteWidth = sizeof(T);
	setUpConstantBuffer(device);
}

template <class T>
void ConstantBuffer::setUpConstantBuffer(ID3D11Device* dev){
	D3D11_BUFFER_DESC cBufferDesc;
	cBufferDesc.ByteWidth = c_byteWidth;
	cBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	cBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cBufferDesc.CPUAccessFlags = 0;
	cBufferDesc.MiscFlags = 0;
	cBufferDesc.StructureByteStride = 0;
	dev->CreateBuffer(
		&cBufferDesc,
		NULL,
		&constantBuffer);
}

template <class T>
ConstantBuffer::~ConstantBuffer(void)
{
	//ReleaseMacro(constantBuffer);
}*/