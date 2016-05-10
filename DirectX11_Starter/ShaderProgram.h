#ifndef _SHADERPROGRAM_H
#define _SHADERPROGRAM_H
#include <d3d11.h>
#include <cstdint>
#include <d3dcompiler.h>
#include <vector>
#include "ConstantBuffer.h"
#include "WindowSingleton.h"
#define ReleaseMacro(x) { if(x){ x->Release(); x = 0; } }

class ShaderProgram{
public:
	ShaderProgram(wchar_t* vs, wchar_t* ps, wchar_t* gs = nullptr, wchar_t* so = nullptr, wchar_t* is = nullptr, const D3D11_SO_DECLARATION_ENTRY * so_entry = nullptr, unsigned int num_entries = 0);
	~ShaderProgram(void);

	__declspec(property(get = getPixelShader)) ID3D11PixelShader* pixelShader;
	__declspec(property(get = getVertexShader)) ID3D11VertexShader* vertexShader;
	__declspec(property(get = getGeometryShader)) ID3D11GeometryShader* geometryShader;
	__declspec(property(get = getStreamouputShader)) ID3D11GeometryShader* streamoutputShader;
	__declspec(property(get = getInitialVertexShader)) ID3D11VertexShader* initialVertexShader;
	__declspec(property(get = getInputLayout)) ID3D11InputLayout* inputLayout;
	__declspec(property(get = getStreamInputLayout)) ID3D11InputLayout* streamInputLayout;

	ID3D11PixelShader* getPixelShader();
	ID3D11VertexShader* getVertexShader();
	ID3D11GeometryShader* getGeometryShader();
	ID3D11VertexShader* getInitialVertexShader();
	ID3D11GeometryShader* getStreamouputShader();
	ID3D11InputLayout* getInputLayout();
	ID3D11InputLayout* getStreamInputLayout();
protected:
protected:
	HRESULT CreateInputLayoutDescFromShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout);
	ID3D11PixelShader *m_ps;
	ID3D11VertexShader *m_vs;
	ID3D11GeometryShader *m_gs;
	ID3D11VertexShader *m_is;
	ID3D11GeometryShader *m_so;
	ID3D11InputLayout *m_il;
	ID3D11InputLayout *m_sl;
	WindowSingleton *wind;
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
public:
	ID3D11PixelShader* m_pixelShader;
	ID3D11VertexShader* m_vertexShader;
	ID3D11GeometryShader* m_geometryShader;
	ID3D11GeometryShader* streamOutputShader;
	ID3D11VertexShader* soVertexShader;
	ID3D11InputLayout* vsInputLayout;
	ID3D11InputLayout* soVSInputLayout;
	ID3D11InputLayout* psInputLayout;
	ID3D11InputLayout* gsInputLayout;
	ID3D11InputLayout* soInputLayout;
	ID3D11InputLayout* camInputLayout;
};
#endif