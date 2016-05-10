#include "Mesh.h"
#include <d3dcompiler.h>
#include "Global.h"
#include <typeinfo>

//model mesh constructor
Mesh::Mesh(Vertex* vertices, UINT* indices, int size)
	:m_vertices(vertices), m_indices(indices), m_size(size),
	m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	//wind = WindowSingleton::Instance();

	sizeofvertex = sizeof(Vertex);
	init_buffer = nullptr;
	so_buffer = nullptr;

	createVertexBuffer();
	createIndexBuffer();
}

Mesh::Mesh(Phong* vertices, UINT* indices, int size)
	:m_vertices(vertices), m_indices(indices), m_size(size),
	m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	//wind = WindowSingleton::Instance();
	sizeofvertex = sizeof(Phong);
	init_buffer = nullptr;
	so_buffer = nullptr;

	createVertexBuffer();
	createIndexBuffer();
}

Mesh::Mesh(Particle* vertices, UINT* indices, int size)
	:m_vertices(vertices), m_indices(indices), m_size(size),
	m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	//wind = WindowSingleton::Instance();

	sizeofvertex = sizeof(Particle);

	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = m_vertices;

	D3D11_BUFFER_DESC sobd;
	sobd.Usage = D3D11_USAGE_DEFAULT;
	//sobd.ByteWidth = sizeof(Particle) * 10;
	sobd.ByteWidth = sizeofvertex * 50;
	sobd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;
	sobd.CPUAccessFlags = 0;
	sobd.MiscFlags = 0;
	sobd.StructureByteStride = 0;
	m_device->CreateBuffer(&sobd, 0, &so_buffer);

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	//sobd.ByteWidth = sizeof(Particle) * 10;
	vbd.ByteWidth = sizeofvertex * 50;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	m_device->CreateBuffer(&vbd, NULL, &v_buffer);

	createIndexBuffer();
	createInitBuffer();
}

void deleteBuffer(ID3D11Buffer* buffer)
{
	if (buffer)
		buffer->Release();
	buffer = 0;
}

Mesh::~Mesh(void)
{
	deleteBuffer(v_buffer);
	deleteBuffer(i_buffer);
	deleteBuffer(init_buffer);
	deleteBuffer(so_buffer);

}

void Mesh::createVertexBuffer(){
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeofvertex * m_size;
	// Number of vertices in the "model" you want to draw
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = m_vertices;
	m_device->CreateBuffer(&vbd, &initialVertexData, &v_buffer);

}

void Mesh::createInitBuffer(){
	D3D11_BUFFER_DESC initbd;
	initbd.Usage = D3D11_USAGE_DEFAULT;
	initbd.ByteWidth = sizeofvertex;
	// Number of vertices in the "model" you want to draw
	initbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	initbd.CPUAccessFlags = 0;// D3D11_CPU_ACCESS_WRITE;
	initbd.MiscFlags = 0;
	initbd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialVertexData;
	initialVertexData.pSysMem = m_vertices;
	m_device->CreateBuffer(&initbd, &initialVertexData, &init_buffer);
}

void Mesh::createIndexBuffer()
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_IMMUTABLE;
	ibd.ByteWidth = sizeof(UINT) * m_size; // Number of indices in the "model" you want to draw
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = 0;
	ibd.MiscFlags = 0;
	ibd.StructureByteStride = 0;
	D3D11_SUBRESOURCE_DATA initialIndexData;
	initialIndexData.pSysMem = m_indices;
	m_device->CreateBuffer(&ibd, &initialIndexData, &i_buffer);
}

