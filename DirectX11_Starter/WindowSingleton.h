#ifndef _WINDOWSINGLETON_H
#define _WINDOWSINGLETON_H

#include <d3d11.h>

class WindowSingleton{
public:
	static WindowSingleton* Instance();
	ID3D11Device* getDevice();
	void initialize(ID3D11Device* dev, ID3D11DeviceContext* devCtx, ID3D11RenderTargetView* renderTargetView, ID3D11DepthStencilView* depthStencilView, int width, int height);
	ID3D11DeviceContext* getDeviceContext();
	ID3D11DeviceContext* getDeferredContext();
	ID3D11RenderTargetView* getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();

	int getWindowWidth();
	int getWindowHeight();

	void setWindowWidth(int w);
	void setWindowHeight(int h);
	__declspec(property(get = getDevice)) ID3D11Device* device;
	__declspec(property(get = getDeviceContext)) ID3D11DeviceContext* deviceContext;
	__declspec(property(get = getDeferredContext)) ID3D11DeviceContext* deferredContext;
	__declspec(property(get = getRenderTargetView)) ID3D11RenderTargetView* renderTargetView;
	__declspec(property(get = getDepthStencilView)) ID3D11DepthStencilView* depthStencilView;
	__declspec(property(get = getWindowWidth, put = setWindowWidth)) int windowWidth;
	__declspec(property(get = getWindowHeight, put = setWindowHeight)) int windowHeight;
protected:
	WindowSingleton();
private:
	static WindowSingleton* _instance;
	static ID3D11Device* m_device;
	static ID3D11DeviceContext* m_deviceContext;
	static ID3D11DeviceContext* m_deferredContext;
	static ID3D11RenderTargetView* m_renderTargetView;
	static ID3D11DepthStencilView* m_depthStencilView;
	static int m_windowWidth;
	static int m_windowHeight;
};

#endif