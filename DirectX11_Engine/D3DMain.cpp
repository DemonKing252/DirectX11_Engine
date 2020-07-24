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

	// Win32 Init - Step 1 - Define a wndclass
	std::shared_ptr<Window> window = std::make_shared<Window>("HWNDClass", hInstance, camera);
	
	if (!window->Register())
	{
		// Win32 Init - Step 2 - Register our wndclass
		MessageBox(0, "Window Register Failed!", "HR ERROR", 0);
		return -1;
	}
	else
	{
		window->Create("DirectX Engine", X, Y, WIDTH, HEIGHT, hInstance, mCmdShow);
	
		/* Now we initialzie Direct3D */

		// D3D Init - Step 1 - Create a device and swap chain
		D3D11App::Instance()->InitDeviceAndSwapChain(window);
		
		// D3D Init - Step 2 - Create our depth and stencil buffers
		D3D11App::Instance()->InitDepthAndStencilView(window);

		// D3D Init - Step 3 - Set the render target as the back buffer
		D3D11App::Instance()->InitRenderTarget(window);

		// Win32 Init - Step 3 - Process the messages that get sent to the window
		MSG msg;
		ZeroMemory(&msg, sizeof(MSG));

		while (!window->m_bQuitMessagePosted)
		{
			window->DispatchMessages(msg);
			
			// Clear render target and depth/stencil buffers
			D3D11App::Instance()->ClearRenderTargetView();
			D3D11App::Instance()->ClearDepthAndStencilView();

			// Update and Draw
			D3D11App::Instance()->Update();
			D3D11App::Instance()->Draw();
			
			/* IDXGISwapChain Presenting */

			// Swap the front and the back buffers using *m_d3dSwapChain->Present()*
			// Note: The first arguement expects a vsync interval. 1 means vsync is turned on, 0 means its turned off 
			D3D11App::Instance()->SwapBuffers();
		}
		// Unregister our window class 
		window->Clean();

		// Release our device, device context, swap chain, and anything else we used to actually draw stuff
		// (ie: vertex buffers, index buffers, vertex shaders, pixel shaders, input elements, etc...)
		D3D11App::Instance()->Clean();
	}
	return 0;
}