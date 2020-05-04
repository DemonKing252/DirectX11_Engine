#include "Window.h"
#include "D3D11Application.h"
/*********************************************************************************
DirectX11_Engine::WinMain.cpp by Liam Blake (C) 2020 All Rights Reserved.

Created: 2020-04-19
Modified: 2020-05-04
Lines of code: 611

*********************************************************************************/
int CALLBACK WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pCmdLine, int mCmdShow)
{
	// D3D11 Engine 

	Window window("HWNDClass", hInstance);

	if (!window.registerClass())
	{
		MessageBox(0, "Window Register Failed!", "HR ERROR", 0);
		return -1;
	}
	else
	{
		window.createWindow("D3D11 Engine", 100, 100, 800, 600, hInstance, mCmdShow);
		
		D3D11App::Get().InitDeviceAndSwapChain(&window);
		D3D11App::Get().InitRenderTarget(&window);

		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (!window.quitMessagePosted)
		{
			window.messageLoop(msg);

			D3D11App::Get().ClearRenderTargetView();
			
			D3D11App::Get().Update();
			D3D11App::Get().Draw();
			
			D3D11App::Get().PresentSwapChain();
		}
		D3D11App::Get().Clean();
	}

	return 0;
}