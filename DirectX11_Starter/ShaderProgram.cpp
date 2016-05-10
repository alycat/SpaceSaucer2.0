#include "ShaderProgram.h"
#include "Global.h"
#include <iostream>
#include "WindowSingleton.h"

ShaderProgram::ShaderProgram(wchar_t* vs, wchar_t* ps, wchar_t* gs, wchar_t* so, wchar_t* is, 
	const D3D11_SO_DECLARATION_ENTRY* so_entry, unsigned int num_entries)
	: m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	m_vs = nullptr;
	ID3DBlob* vsBlob;
	D3DReadFileToBlob(vs, &vsBlob);
	HRESULT hr_vertex = this->CreateInputLayoutDescFromShaderSignature(vsBlob, m_device, &m_il);
	m_device->CreateVertexShader(vsBlob->GetBufferPointer(), vsBlob->GetBufferSize(), NULL, &m_vs);
	ReleaseMacro(vsBlob);

	m_ps = nullptr;
	ID3DBlob* psBlob;
	D3DReadFileToBlob(ps, &psBlob);
	m_device->CreatePixelShader(psBlob->GetBufferPointer(), psBlob->GetBufferSize(), NULL, &m_ps);
	ReleaseMacro(psBlob);

	m_gs = nullptr;
	if (gs)
	{
		ID3DBlob* gsBlob;
		D3DReadFileToBlob(gs, &gsBlob);
		m_device->CreateGeometryShader(gsBlob->GetBufferPointer(), gsBlob->GetBufferSize(), NULL, &m_gs);
		ReleaseMacro(gsBlob);
	}
	m_is = nullptr;
	m_sl = nullptr;
	if (is)
	{
		ID3DBlob *isBlob;
		D3DReadFileToBlob(is, &isBlob);
		this->CreateInputLayoutDescFromShaderSignature(isBlob, m_device, &m_sl);
		m_device->CreateVertexShader(isBlob->GetBufferPointer(), isBlob->GetBufferSize(), NULL, &m_is);
		ReleaseMacro(isBlob);
	}
	m_so = nullptr;
	if (so_entry && so)
	{
		ID3DBlob* soBlob;
		D3DReadFileToBlob(so, &soBlob);
		UINT stride = sizeof(so_entry);	
		HRESULT hr = m_device->CreateGeometryShaderWithStreamOutput(soBlob->GetBufferPointer(), soBlob->GetBufferSize(), so_entry, num_entries, NULL, 0, NULL, NULL, &m_so);
		
		if (FAILED(hr)){
			OutputDebugString(L"Failed To Create Stream output Shader\n");
			//dev->CreateGeometryShader(soBlob->GetBufferPointer(), soBlob->GetBufferSize(), NULL, &geometryShader);
		}
		ReleaseMacro(soBlob);
	}
	gsInputLayout = nullptr;
}

ID3D11PixelShader* ShaderProgram::getPixelShader(){ return m_ps; }
ID3D11VertexShader* ShaderProgram::getVertexShader(){ return m_vs; }
ID3D11GeometryShader* ShaderProgram::getGeometryShader(){ return m_gs; }
ID3D11VertexShader* ShaderProgram::getInitialVertexShader(){ return m_is; }
ID3D11GeometryShader* ShaderProgram::getStreamouputShader(){ return m_so; }
ID3D11InputLayout* ShaderProgram::getInputLayout(){ return m_il; }
ID3D11InputLayout* ShaderProgram::getStreamInputLayout(){ return m_sl; }

template <class T>
void deleteShader(T* shader)
{
	if (shader)
		shader->Release();
	shader = 0;
}

ShaderProgram::~ShaderProgram(void)
{
	deleteShader<ID3D11VertexShader>(m_vs);
	deleteShader<ID3D11PixelShader>(m_ps);
	deleteShader<ID3D11GeometryShader>(m_gs);
	deleteShader<ID3D11VertexShader>(m_is);
	deleteShader<ID3D11GeometryShader>(m_so);
	//deleteHelper<ID3D11InputLayout>(m_il);
	//deleteHelper<ID3D11InputLayout>(m_sl);
}


/**
*http://takinginitiative.wordpress.com/2011/12/11/directx-1011-basic-shader-reflection-automatic-input-layout-creation/
**/
HRESULT ShaderProgram::CreateInputLayoutDescFromShaderSignature(ID3DBlob* pShaderBlob, ID3D11Device* pD3DDevice, ID3D11InputLayout** pInputLayout)
{
	// Reflect shader info
	ID3D11ShaderReflection* pVertexShaderReflection = NULL;
	if (FAILED(D3DReflect(pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), IID_ID3D11ShaderReflection, (void**)&pVertexShaderReflection)))
	{
		return S_FALSE;
	}

	// Get shader info
	D3D11_SHADER_DESC shaderDesc;
	pVertexShaderReflection->GetDesc(&shaderDesc);

	// Read input layout description from shader info
	std::vector<D3D11_INPUT_ELEMENT_DESC> inputLayoutDesc;
	for (uint32_t i = 0; i< shaderDesc.InputParameters; i++)
	{
		D3D11_SIGNATURE_PARAMETER_DESC paramDesc;
		pVertexShaderReflection->GetInputParameterDesc(i, &paramDesc);

		// fill out input element desc
		D3D11_INPUT_ELEMENT_DESC elementDesc;
		elementDesc.SemanticName = paramDesc.SemanticName;
		elementDesc.SemanticIndex = paramDesc.SemanticIndex;
		elementDesc.InputSlot = 0;
		elementDesc.AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
		elementDesc.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
		elementDesc.InstanceDataStepRate = 0;

		// determine DXGI format
		if (paramDesc.Mask == 1)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32_FLOAT;
		}
		else if (paramDesc.Mask <= 3)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32_FLOAT;
		}
		else if (paramDesc.Mask <= 7)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32_FLOAT;
		}
		else if (paramDesc.Mask <= 15)
		{
			if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_UINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_UINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_SINT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_SINT;
			else if (paramDesc.ComponentType == D3D_REGISTER_COMPONENT_FLOAT32) elementDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		}

		//save element desc
		inputLayoutDesc.push_back(elementDesc);
	}

	// Try to create Input Layout
	HRESULT hr = pD3DDevice->CreateInputLayout(&inputLayoutDesc[0], inputLayoutDesc.size(), pShaderBlob->GetBufferPointer(), pShaderBlob->GetBufferSize(), pInputLayout);

	//Free allocation shader reflection memory
	pVertexShaderReflection->Release();
	return hr;
}