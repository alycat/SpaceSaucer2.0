#include "WindowSingleton.h"

WindowSingleton* WindowSingleton::_instance = nullptr;
ID3D11Device* WindowSingleton::m_device = nullptr;
ID3D11DeviceContext* WindowSingleton::m_deviceContext = nullptr;
ID3D11DeviceContext* WindowSingleton::m_deferredContext = nullptr;
ID3D11RenderTargetView* WindowSingleton::m_renderTargetView = nullptr;
ID3D11DepthStencilView* WindowSingleton::m_depthStencilView = nullptr;
int WindowSingleton::m_windowWidth = 0;
int WindowSingleton::m_windowHeight = 0;

WindowSingleton::WindowSingleton()
{

}


WindowSingleton* WindowSingleton::Instance()
{
	if (_instance == nullptr)
	{
		_instance = new WindowSingleton();
	}
	return _instance;
}

ID3D11Device* WindowSingleton::getDevice(){ return m_device; }
ID3D11DeviceContext* WindowSingleton::getDeviceContext(){ return m_deviceContext; }
ID3D11DeviceContext* WindowSingleton::getDeferredContext(){ return m_deferredContext; }
ID3D11RenderTargetView* WindowSingleton::getRenderTargetView(){ return m_renderTargetView; }
ID3D11DepthStencilView* WindowSingleton::getDepthStencilView(){ return m_depthStencilView; }
int WindowSingleton::getWindowWidth(){ return m_windowWidth; }
int WindowSingleton::getWindowHeight(){ return m_windowHeight; }
void WindowSingleton::setWindowWidth(int w){ m_windowWidth = w; }
void WindowSingleton::setWindowHeight(int h){ m_windowHeight = h; }

void WindowSingleton::initialize(ID3D11Device* dev, ID3D11DeviceContext* devCtx, 
	ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView,
	int width, int height)
{
	m_device = dev;
	m_deviceContext = devCtx;
	m_device->CreateDeferredContext(0, &m_deferredContext);
	m_renderTargetView = renderTargetView;
	m_depthStencilView = depthStencilView;
	m_windowWidth = width;
	m_windowHeight = height;
}