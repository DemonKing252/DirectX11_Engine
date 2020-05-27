#pragma once
#include "D3DCommon.h"
#include "Graphics.h"
// Remove rough edges off sides of geometry. Sometimes called the "stair case effect".
#define ANTI_ALIASING_SAMPLE_COUNT 4
#define ANTI_ALIASING_QUALITY_COUNT 0
class Window;

typedef class D3D11Application
{
public:
	D3D11Application(D3D11Application* copy) = delete;

	// Initalizing D3D
	void InitDeviceAndSwapChain(std::shared_ptr<Window> window)const;
	void InitDepthAndStencilView(std::shared_ptr<Window> window)const;
	void InitRenderTarget(std::shared_ptr<Window> window)const;
	
	// Game loop
	void ClearRenderTargetView();
	void ClearDepthAndStencilView();
	void Update();
	void Draw();
	void EndFrame();

	void Clean();

	~D3D11Application();
	D3D11Application();

	static D3D11Application& Get()
	{
		return s_Instance;
	}

	mutable FLOAT clear_color[4];

private:
	static D3D11Application s_Instance;
	mutable Graphics gfx;
	// Main device adapters 
	mutable Microsoft::WRL::ComPtr<IDXGISwapChain> m_d3dSwapChain;
	mutable Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	mutable Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	mutable Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dBackBuffer;

	
	// Depth and Stencil
	mutable Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3dDepthStencilView;
	
} D3D11Application, D3D11App, D3D11Engine;