#ifndef _MATERIAL_H
#define _MATERIAL_H

#include <d3d11.h>
#include <Windows.h>
#include <DirectXMath.h>
#include "Global.h"
#include <d3dcompiler.h>
#include <vector>
#include <cstdint>
#include "dxerr.h"
#include "ShaderProgram.h"
#include <cstdarg>
#include <stdarg.h>
#include "WindowSingleton.h"
using namespace DirectX;

class Material{
public:
	ID3D11ShaderResourceView* resourceView;
	ID3D11ShaderResourceView* resourceView2;
	ID3D11ShaderResourceView* resourceView3;
	ID3D11ShaderResourceView** resources;
	ID3D11SamplerState* samplerState;

	int getResourceCount();
	__declspec(property(get = getResourceCount)) int resourceCount;

	// Our basic shaders for this example
	ShaderProgram* shaderProgram;

	ID3D11Buffer* vsConstantBuffer;
	ID3D11Buffer* psConstantBuffer;
	Material(ID3D11ShaderResourceView* rv);
	Material(int files, ...);
	Material(ID3D11ShaderResourceView* rv, int files, ...);
	~Material(void);
protected:
	WindowSingleton* wind;
	int m_resourceCount;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
};

#endif