#include "Window.h"
#include "D3D11Application.h"
#include "Camera.h"
#include <memory>

#define X 100
#define Y 100
#define WIDTH 800
#define HEIGHT 600
/*********************************************************************************
DirectX11_Engine::D3DMain.cpp by Liam Blake (C) 2020 All Rights Reserved.

Created: 2020-04-19
Modified: 2020-05-27
Lines of code: 1645

*********************************************************************************/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int mCmdShow)
{
	// Must be a shared pointer because we have a copy of these instances in D3DApp
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	std::shared_ptr<Window> window = std::make_shared<Window>("HWNDClass", hInstance, camera);
	
	if (!window->Register())
	{
		MessageBox(0, "Window Register Failed!", "HR ERROR", 0);
		return -1;
	}
	else
	{
		window->Create("D3D11 Engine", X, Y, WIDTH, HEIGHT, hInstance, mCmdShow);
		
		D3D11App::Get().InitDeviceAndSwapChain(window);
		D3D11App::Get().InitDepthAndStencilView(window);
		D3D11App::Get().InitRenderTarget(window);

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (!window->m_bQuitMessagePosted)
		{
			window->MessageLoop(msg);
			
			D3D11App::Get().ClearRenderTargetView();
			D3D11App::Get().ClearDepthAndStencilView();

			D3D11App::Get().Update();
			D3D11App::Get().Draw();
			
			D3D11App::Get().EndFrame();
		}
		window->Clean();
		D3D11App::Get().Clean();
	}
	return 0;
}