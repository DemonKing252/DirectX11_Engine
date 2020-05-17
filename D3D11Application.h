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
	void InitDeviceAndSwapChain(const Window* window);
	void InitDepthAndStencilView(const Window* window);
	void InitRenderTarget(Window* window);
	
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
	Graphics gfx;

	// Main device adapters 
	IDXGISwapChain* m_d3dSwapChain;
	ID3D11Device* m_d3dDevice;
	ID3D11DeviceContext* m_d3dDeviceContext;
	ID3D11RenderTargetView* m_d3dBackBuffer;

	// Keyboard and mouse

	// Depth and Stencil
	ID3D11DepthStencilView* m_d3dDepthStencilView;
	
} D3D11Application, D3D11App, D3D11Engine;

