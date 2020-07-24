#pragma once
#include "D3DCommon.h"
#include "Graphics.h"
// Remove rough edges off sides of geometry. Sometimes called the "stair case effect".
#define ANTI_ALIASING_SAMPLE_COUNT 4
#define ANTI_ALIASING_QUALITY_COUNT 0
class Window;

typedef class D3D11Application
{
private:
	const INT BufferCount = 2;
	static D3D11Application* s_pInstance;
	mutable Graphics gfx;
	
	// D3D Graphics Infrastructure Swap Chain
	mutable Microsoft::WRL::ComPtr<IDXGISwapChain> m_dxgiSwapChain;
	
	// Main device adapters 
	mutable Microsoft::WRL::ComPtr<ID3D11Device> m_d3dDevice;
	mutable Microsoft::WRL::ComPtr<ID3D11DeviceContext> m_d3dDeviceContext;
	
	// Depth and Stencil
	mutable Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_d3dDepthStencilView;

	// Render Target View
	mutable Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_d3dBackBuffer;

	// Rasterizer State
	mutable Microsoft::WRL::ComPtr<ID3D11RasterizerState> m_d3dRasterizerState;
public:
	D3D11Application(D3D11Application* copy) = delete;

	// Initializeing D3D Methods
	void InitDeviceAndSwapChain(std::shared_ptr<Window> window)const;
	void InitDepthAndStencilView(std::shared_ptr<Window> window)const;
	void InitRenderTarget(std::shared_ptr<Window> window)const;
	
	// Game loop
	void ClearRenderTargetView();
	void ClearDepthAndStencilView();
	void Update();
	void Draw();
	void SwapBuffers();

	void Clean();

	~D3D11Application();
	D3D11Application();

	static D3D11Application* Instance()
	{
		if (s_pInstance == nullptr)
		{
			s_pInstance = new D3D11Application();
			return s_pInstance;
		}
		else
			return s_pInstance;
	}

	mutable FLOAT m_f4ClearColor[4];
} D3D11Application, D3D11App, D3D11Engine;