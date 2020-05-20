#include "D3D11Application.h"
#include "Window.h"
D3D11Application D3D11Application::s_Instance = D3D11Application();

void D3D11Application::InitDeviceAndSwapChain(std::shared_ptr<Window> window) const
{
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 1;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.Width  = window->getWindowSize().x;
	swapChainDesc.BufferDesc.Height = window->getWindowSize().y;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->GetWindow();
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

void D3D11Application::InitDepthAndStencilView(std::shared_ptr<Window> window) const
{
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthDesc.Width =  window->getWindowSize().x;
	depthDesc.Height = window->getWindowSize().y;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = 4;			// must match up with swap chain desc
	depthDesc.SampleDesc.Quality = 0;		// must match up with swap chain desc
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthDesc.CPUAccessFlags = 0;
	depthDesc.MiscFlags = 0;

	ID3D11Texture2D* depthStencilTexture;
	ZeroMemory(&depthStencilTexture, sizeof(ID3D11Texture2D));

	ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthDesc, NULL, &depthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));
	depthStencilDesc.Format = depthDesc.Format;
	depthStencilDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencilTexture, NULL, m_d3dDepthStencilView.GetAddressOf()));
	depthStencilTexture->Release();
}

void D3D11Application::InitRenderTarget(std::shared_ptr<Window> window) const
{ 
	ID3D11Texture2D* pBackBuffer;
	ZeroMemory(&pBackBuffer, sizeof(ID3D11Texture2D));

	// get the back buffer address
	m_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));

	// create a render target using the back buffer
	ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, m_d3dBackBuffer.GetAddressOf()));
	pBackBuffer->Release();

	// set the render target as the back buffer
	m_d3dDeviceContext->OMSetRenderTargets(1, m_d3dBackBuffer.GetAddressOf(), m_d3dDepthStencilView.Get());

	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->getWindowSize().x;
	viewport.Height = window->getWindowSize().y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	m_d3dDeviceContext->RSSetViewports(1, &viewport);

	gfx.InitPipeline(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
	gfx.InitGraphics(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
}

void D3D11Application::ClearRenderTargetView()
{
	FLOAT clear_color[] = { 0.0f, 0.0f, 0.2f, 1.0f };

	m_d3dDeviceContext->ClearRenderTargetView(m_d3dBackBuffer.Get(), clear_color);
}

void D3D11Application::ClearDepthAndStencilView()
{
	m_d3dDeviceContext->ClearDepthStencilView(m_d3dDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
}

void D3D11Application::Update()
{
	gfx.Update(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
}

void D3D11Application::Draw()
{
	gfx.Draw(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
}

void D3D11Application::PresentSwapChain()
{
	m_d3dSwapChain->Present(0, 0);
}

void D3D11Application::Clean()
{
	gfx.Clean();
	
	m_d3dDepthStencilView.Reset();
	m_d3dDevice.Reset();
	m_d3dDeviceContext.Reset();
	m_d3dBackBuffer.Reset();
	m_d3dSwapChain.Reset();
}

D3D11Application::~D3D11Application()
{
}

D3D11Application::D3D11Application()
{
}