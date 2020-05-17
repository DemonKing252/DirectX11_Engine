#pragma once
#include <iostream>
#include <array>
#include <cmath>
#include "D3DCommon.h"
#include "Graphics.h"
class Window;

typedef class D3D11Application
{
public:
	D3D11Application(D3D11Application* other) = delete;

	// Initalizing D3D
	void InitDeviceAndSwapChain(std::shared_ptr<Window> window)const;
	void InitDepthAndStencilView(std::shared_ptr<Window> window)const;
	void InitRenderTarget(std::shared_ptr<Window> window)const;
	
	// Game loop
	void ClearRenderTargetView();
	void ClearDepthAndStencilView();
	void Update();
	void Draw();
	void PresentSwapChain();

	void Clean();

	~D3D11Application();
	D3D11Application();

	static D3D11Application& Get()
	{
		return s_Instance;
	}

private:
	static D3D11Application s_Instance;
	mutable Graphics gfx;

	// Main device adapters 
	mutable IDXGISwapChain* m_d3dSwapChain;
	mutable ID3D11Device* m_d3dDevice;
	mutable ID3D11DeviceContext* m_d3dDeviceContext;
	mutable ID3D11RenderTargetView* m_d3dBackBuffer;

	// Keyboard and mouse

	// Depth and Stencil
	mutable ID3D11DepthStencilView* m_d3dDepthStencilView;
	
} D3D11Application, D3D11App, D3D11Engine;