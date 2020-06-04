#include "D3D11Application.h"
#include "Window.h"
D3D11Application D3D11Application::s_Instance = D3D11Application();

void D3D11Application::InitDeviceAndSwapChain(std::shared_ptr<Window> window) const
{
	// Step6: Create a swap chain to manage presenting the front and back buffers
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = 2;
	swapChainDesc.BufferDesc.Width  = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.OutputWindow = window->GetWindow();
	swapChainDesc.SampleDesc.Count = ANTI_ALIASING_SAMPLE_COUNT;
	swapChainDesc.SampleDesc.Quality = ANTI_ALIASING_QUALITY_COUNT;
	swapChainDesc.Windowed = TRUE;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;

	
	// Step7: Create our device, device context, and swap chain
	ThrowIfFailed(D3D11CreateDeviceAndSwapChain
	(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		D3D11_CREATE_DEVICE_DEBUG, // you can turn off debug erros by just passing NULL.
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
	// Enable depth and stencil buffers
	D3D11_TEXTURE2D_DESC depthDesc;
	ZeroMemory(&depthDesc, sizeof(D3D11_TEXTURE2D_DESC));
	depthDesc.Width = window->GetFrameBufferSize().x;
	depthDesc.Height = window->GetFrameBufferSize().y;
	depthDesc.MipLevels = 1;
	depthDesc.ArraySize = 1;
	depthDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthDesc.SampleDesc.Count = ANTI_ALIASING_SAMPLE_COUNT;	// must match up with swap chain desc
	depthDesc.SampleDesc.Quality = ANTI_ALIASING_QUALITY_COUNT;	// must match up with swap chain desc
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
	// Step8: Get the back buffer address in the swap chain and set it as the render target
	ID3D11Texture2D* pBackBuffer;
	ZeroMemory(&pBackBuffer, sizeof(ID3D11Texture2D));

	/* Step 8-1 */
	// get the back buffer address
	m_d3dSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer));

	/* Step 8-2 */
	// create a render target using the back buffer
	ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(pBackBuffer, NULL, m_d3dBackBuffer.GetAddressOf()));
	pBackBuffer->Release();

	/* Step 8-3 */
	// set the render target as the back buffer
	m_d3dDeviceContext->OMSetRenderTargets(1, m_d3dBackBuffer.GetAddressOf(), m_d3dDepthStencilView.Get());

	// Step9: View port for drawing 
	// setting our desired viewport for our render target.
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = window->GetFrameBufferSize().x;
	viewport.Height = window->GetFrameBufferSize().y;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	m_d3dDeviceContext->RSSetViewports(1, &viewport);
	
	m_f4ClearColor[Color::R] = 0.0f;
	m_f4ClearColor[Color::G] = 0.0f;
	m_f4ClearColor[Color::B] = 0.2f;
	m_f4ClearColor[Color::A] = 1.0f;


	gfx.InitPipeline(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
	gfx.InitGraphics(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
	gfx.InitImGui(m_d3dDevice.Get(), m_d3dDeviceContext.Get(), window);
}

void D3D11Application::ClearRenderTargetView()
{
	// Step10: Clear the render target
	// clearing the back buffer and filling it with a background color
	m_d3dDeviceContext->ClearRenderTargetView(
		m_d3dBackBuffer.Get(), 
		m_f4ClearColor
	);
}

void D3D11Application::ClearDepthAndStencilView()
{
	// clear depth and stencil buffers (only depth matters at this point)
	m_d3dDeviceContext->ClearDepthStencilView(
		m_d3dDepthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0
	);
}

void D3D11Application::Update()
{
	gfx.Update(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
}

void D3D11Application::Draw()
{
	gfx.Draw(m_d3dDevice.Get(), m_d3dDeviceContext.Get());
}

void D3D11Application::EndFrame()
{
	// Step11: Present the back buffer to the screen, send the front buffer to the back for the next frame
	m_d3dSwapChain->Present(static_cast<INT>(gfx.VsyncEnabled()), 0);
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