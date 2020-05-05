#include "D3D11Application.h"
#include "Window.h"
D3D11Application D3D11Application::s_pInstance = D3D11Application();

void D3D11Application::InitDeviceAndSwapChain(const Window* window)
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width  = window->getWindowSize().x;
	swapChainDesc.BufferDesc.Height = window->getWindowSize().y;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->getWindow();
	swapChainDesc.SampleDesc.Count = 4;
	swapChainDesc.SampleDesc.Quality = 0;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	
	ThrowIfFailed(D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG,
		NULL,
		NULL,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&m_d3dSwapChain,
		&m_d3dDevice,
		NULL,
		&m_d3dDeviceContext
	));
}


void D3D11Application::InitRenderTarget(const Window* window)
{
	ID3D11Texture2D* pBackBuffer;
	ZeroMemory(&pBackBuffer, sizeof(ID3D11Texture2D));

	// get the back buffer address
	m_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));

	// create a render target using the back buffer
	ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_d3dBackBuffer));
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_d3dDeviceContext->OMSetRenderTargets(1, &m_d3dBackBuffer, NULL);

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->getWindowSize().x;
	viewport.Height = window->getWindowSize().y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	m_d3dDeviceContext->RSSetViewports(1, &viewport);

	gfx.InitPipeline(m_d3dDevice, m_d3dDeviceContext);
	gfx.InitGraphics(m_d3dDevice, m_d3dDeviceContext);

}

void D3D11Application::ClearRenderTargetView()
{
	FLOAT rgba[] = { 15.0f /255.0f, 29.0f /255.0f, 70.0f /255.0f, 1.0f };

	m_d3dDeviceContext->ClearRenderTargetView(m_d3dBackBuffer, rgba);
}

void D3D11Application::Update()
{
	gfx.Update(m_d3dDevice, m_d3dDeviceContext);

	//if (m_mouse.Get().GetState().leftButton)
	{
	//	exit(0);
	}
}

void D3D11Application::Draw()
{
	gfx.Draw(m_d3dDeviceContext);
}

void D3D11Application::PresentSwapChain()
{
	m_d3dSwapChain->Present(0, 0);
}

void D3D11Application::Clean()
{
	gfx.Clean();

	m_d3dDevice->Release();
	m_d3dDeviceContext->Release();
	m_d3dBackBuffer->Release();
}

D3D11Application::~D3D11Application()
{
}

D3D11Application::D3D11Application()
{
}
