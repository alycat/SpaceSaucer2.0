#ifndef _MESH_H
#define _MESH_H

#include "Global.h"
#include <Windows.h>
#include <d3d11.h>
#include "WindowSingleton.h"

class Mesh{
public:
	int m_size;
	void* m_vertices;
	UINT* m_indices;
	ID3D11Buffer* v_buffer;
	ID3D11Buffer* so_buffer;
	ID3D11Buffer* i_buffer;
	ID3D11Buffer* init_buffer;
	int sizeofvertex;
	Mesh(Vertex* vertices, UINT* indices, int size);
	//Mesh(Vertex* vertices, UINT* indices, int size, ID3D11Device* device);
	Mesh(Phong* vertices, UINT* indices, int size);
	Mesh(Particle* vertices, UINT* indices, int size);
	~Mesh(void);
	void createVertexBuffer();
	void createIndexBuffer();
	void createInitBuffer();
	void drawMesh(ID3D11DeviceContext* deviceContext);
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
private:
	WindowSingleton* wind;
};

#endif