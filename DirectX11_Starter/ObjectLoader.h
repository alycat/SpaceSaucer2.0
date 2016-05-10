//Referenced http://www.braynzarsoft.net/Code/index.php?p=VC&code=Obj-Model-Loader while coding this class
#pragma once
#include "Mesh.h"
#include "Global.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
using namespace DirectX;

class ObjectLoader{

public:
	std::vector<Vertex> vertices;
	std::vector<UINT> indices;
	std::vector<XMFLOAT3> Positions;
	std::vector<XMFLOAT2> UVs;
	std::vector<XMFLOAT3> Normals;
	std::vector<UINT> splitString(std::string in);
	ObjectLoader();
	~ObjectLoader();
	bool CompareVertices(Vertex a, Vertex b);
	UINT GetIndex(Vertex v);
	Mesh* LoadModel(std::string file);
	Vertex* VecToArray();
	XMFLOAT3* CalculateTangent(Triangle& triangle);
	float XMFLOATDotProduct(XMFLOAT3 first, XMFLOAT3 second);
};