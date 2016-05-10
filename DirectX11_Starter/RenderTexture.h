#ifndef _RenderTexture_H_
#define _RenderTexture_H_
#include <d3d11.h>
#include "WindowSingleton.h"
#include <vector>
class RenderTexture
{
public:
	RenderTexture(int bufferCount = 1);
	//RenderTexture(const RenderTexture&);
	~RenderTexture();

	bool Initialize(int textureWidth, int textureHeight);
	void Shutdown();

	void SetRenderTarget(ID3D11DepthStencilView* depthStencilView);
	void GetBackBufferRenderTarget(ID3D11DepthStencilView* depthStencilView);
	void SetBackBufferRenderTarget(ID3D11DepthStencilView* depthStencilView);
	void ClearRenderTarget(ID3D11DepthStencilView* depthStencilView, const FLOAT rgbaColor[4]);
	ID3D11ShaderResourceView* GetShaderResourceView(int index = 0);
	ID3D11RenderTargetView** getRenderTargetView();
	ID3D11DepthStencilView* getDepthStencilView();
	ID3D11Device* m_device;
	ID3D11DeviceContext* m_deviceContext;
private:
	std::vector<ID3D11Texture2D*> m_renderTargetTexture;
	std::vector<ID3D11RenderTargetView*> m_renderTargetView;
	std::vector<ID3D11ShaderResourceView*> m_shaderResourceView;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilView* m_depthStencilView;
	D3D11_VIEWPORT m_viewport;
	int m_bufferCount;
	WindowSingleton* wind;
};

#endif