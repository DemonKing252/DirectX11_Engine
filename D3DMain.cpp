#include "Window.h"
#include "D3D11Application.h"
#include "Camera.h"
#include <memory>
/*********************************************************************************
DirectX11_Engine::D3DMain.cpp by Liam Blake (C) 2020 All Rights Reserved.

Created: 2020-04-19
Modified: 2020-05-16
Lines of code: 1134

*********************************************************************************/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int mCmdShow)
{
	// Must be a shared pointer because we have a copy of these instances in D3DApp
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	std::shared_ptr<Window> window = std::make_shared<Window>("HWNDClass", hInstance, camera);
	
	if (!window->RegisterWinClass())
	{
		MessageBox(0, "Window Register Failed!", "HR ERROR", 0);
		return -1;
	}
	else
	{
		window->CreateWin32Window("D3D11 Engine", 100, 100, 800, 600, hInstance, mCmdShow);
		
		D3D11Engine::Get().InitDeviceAndSwapChain(window);
		D3D11Engine::Get().InitDepthAndStencilView(window);
		D3D11Engine::Get().InitRenderTarget(window);

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (!window->m_bQuitMessagePosted)
		{
			window->MessageLoop(msg);

			D3D11Engine::Get().ClearRenderTargetView();
			D3D11Engine::Get().ClearDepthAndStencilView();

			D3D11Engine::Get().Update();
			D3D11Engine::Get().Draw();
			
			D3D11Engine::Get().PresentSwapChain();
		}
		D3D11Engine::Get().Clean();
	}
	return 0;
}