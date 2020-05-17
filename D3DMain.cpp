#include "Window.h"
#include "D3D11Application.h"
#include "Camera.h"
/*********************************************************************************
DirectX11_Engine::D3DMain.cpp by Liam Blake (C) 2020 All Rights Reserved.

Created: 2020-04-19
Modified: 2020-05-15
Lines of code: 935

*********************************************************************************/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int mCmdShow)
{
	// D3D11 Engine 
	Camera camera;
	Window window("HWNDClass", hInstance, camera);

	if (!window.RegisterWinClass())
	{
		MessageBox(0, "Window Register Failed!", "HR ERROR", 0);
		return -1;
	}
	else
	{
		window.CreateWin32Window("D3D11 Engine", 100, 100, 800, 600, hInstance, mCmdShow);
		
		D3D11Engine::Get().InitDeviceAndSwapChain(&window);
		D3D11Engine::Get().InitDepthAndStencilView(&window);
		D3D11Engine::Get().InitRenderTarget(&window);

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (!window.m_bQuitMessagePosted)
		{
			window.MessageLoop(msg);

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