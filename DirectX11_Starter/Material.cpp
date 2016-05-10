#include "Material.h"
#include "WICTextureLoader.h"
#include "Global.h"
/**
*Material Constructor for materials read/created
*rv: shader resource view of material
*sample: sampler state of material
**/
Material::Material(ID3D11ShaderResourceView* rv) : m_resourceCount(1),
m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	resourceView = nullptr;
	resourceView2 = nullptr;
	resourceView3 = nullptr;
	resources = new ID3D11ShaderResourceView*[1];
	resources[0] = rv;
}

/**
*Material Contructor that creates a texture from an image file
*dev: Device object
*devCtx: deviceContext object
*sampler: sampler state object
*filepath: address of image file
**/
Material::Material(int files, ...) : m_resourceCount(files),
m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	resourceView = nullptr;
	resourceView2 = nullptr;
	resourceView3 = nullptr;
	//wind = WindowSingleton::Instance();
	va_list texture_arguments;
	va_start(texture_arguments, files);
	resources = new ID3D11ShaderResourceView*[files];
	for (unsigned int f = 0; f < files; ++f)
	{
		wchar_t* filepath = va_arg(texture_arguments, wchar_t*);
		CreateWICTextureFromFile(m_device, m_deviceContext, filepath, 0, &resources[f], 0);
	}
	va_end(texture_arguments);
}

Material::Material(ID3D11ShaderResourceView* rv, int files, ...) : m_resourceCount(files + 1),
m_device(wind->device), m_deviceContext(wind->deviceContext)
{
	resourceView = nullptr;
	resourceView2 = nullptr;
	resourceView3 = nullptr;
	//wind = WindowSingleton::Instance();
	va_list texture_arguments;
	va_start(texture_arguments, files);
	resources = new ID3D11ShaderResourceView*[m_resourceCount];
	resources[0] = rv;
	for (unsigned int f = 1; f < m_resourceCount; ++f)
	{
		wchar_t* filepath = va_arg(texture_arguments, wchar_t*);
		CreateWICTextureFromFile(m_device, m_deviceContext, filepath, 0, &resources[f], 0);
	}
	va_end(texture_arguments);
}

Material::~Material(void)
{/*
	for (int i = m_resourceCount - 1; i >= 0; ++i)
	{
		if (resources[i])
			delete resources[i];
		resources[i] = nullptr;
	}
	if (resourceView)
		delete resourceView;
	resourceView = nullptr;
	if (resourceView2)
		delete resourceView2;
	resourceView2 = nullptr;
	if (resourceView3)
		delete resourceView3;
	resourceView3 = nullptr;*/
}

int Material::getResourceCount(){ return m_resourceCount; }