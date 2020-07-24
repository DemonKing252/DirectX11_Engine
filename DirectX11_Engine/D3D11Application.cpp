#include "D3D11Application.h"
#include "Window.h"
D3D11Application* D3D11Application::s_pInstance = nullptr;

void D3D11Application::InitDeviceAndSwapChain(std::shared_ptr<Window> window) const
{
	/* Step 6...9 Initializing Direct3D*/

	// 1. Initialize the swap chain.
	// 2. Get the back buffer address.
	// 3. Create a render target using the back buffer address.
	// 4. Set the back buffer as the render target.

	// Step6: Create a swap chain to manage presenting the front and back buffers
	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));

	swapChainDesc.BufferCount = BufferCount;						// How many buffers will we have?
	swapChainDesc.BufferDesc.Width = NULL;							// Passing in NULL will use the default frame buffer width
	swapChainDesc.BufferDesc.Height = NULL;							// Passing in NULL will use the default frame buffer height
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// Pixel format (RGBA 32 bit is ideal)
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;	// Allow the front and back buffers to be swapped
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;	// Were using the back buffer in the swap chain as the render target
	swapChainDesc.OutputWindow = window->GetWindow();				// HWND instance that we want to use

	/* Anti Aliasing mode */
	// Linus Tech Tips on Anti Aliasing
	// Watch here -> https://www.youtube.com/watch?v=hqi0114mwtY
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
		&m_dxgiSwapChain,
		&m_d3dDevice,
		NULL,
		&m_d3dDeviceContext
	));
}

void D3D11Application::InitDepthAndStencilView(std::shared_ptr<Window> window) const
{
	// Enable depth and stencil buffers
	D3D11_TEXTURE2D_DESC depthStencilDesc;
	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.ArraySize = 1;
	depthStencilDesc.MipLevels = 1;
	depthStencilDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.Width = window->GetFrameBufferSize().x;
	depthStencilDesc.Height = window->GetFrameBufferSize().y;
	depthStencilDesc.CPUAccessFlags = 0;
	depthStencilDesc.Usage = D3D11_USAGE_DEFAULT;
	depthStencilDesc.SampleDesc.Count = ANTI_ALIASING_SAMPLE_COUNT;
	depthStencilDesc.SampleDesc.Quality = ANTI_ALIASING_QUALITY_COUNT;
	depthStencilDesc.MiscFlags = 0;
	depthStencilDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ID3D11Texture2D* pDepthStencilTexture;
	ZeroMemory(&pDepthStencilTexture, sizeof(ID3D11Texture2D));

	// Create a texture format with our depth stencil view
	ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, NULL, &pDepthStencilTexture));

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(D3D11_DEPTH_STENCIL_VIEW_DESC));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	// Create our depth stencil view
	ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(pDepthStencilTexture, &depthStencilViewDesc, m_d3dDepthStencilView.GetAddressOf()))
}

void D3D11Application::InitRenderTarget(std::shared_ptr<Window> window) const
{ 
	// Step8: Get the back buffer address in the swap chain and set it as the render target
	ID3D11Texture2D* pBackBuffer;
	ZeroMemory(&pBackBuffer, sizeof(ID3D11Texture2D));
	
	/* Step 8-1 */
	// get the back buffer address
	ThrowIfFailed(m_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<LPVOID*>(&pBackBuffer)));
	/*
		Liam's Notes:

		In DirectX 12 we would have to create a descriptor heap which holds the information of each render target.
		Due to this, we would have to create a render target for each frame and determine which frame index we are at
		and use that CPU descriptor handle at said index to clear the render target. Much more complicated! This is why you should
		learn DirectX 11 before learning DirectX 12!!!
	*/

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

	/* DirectX Rasterizer Settings */
	D3D11_RASTERIZER_DESC rasterizerStateDesc;
	ZeroMemory(&rasterizerStateDesc, sizeof(D3D11_RASTERIZER_DESC));

	rasterizerStateDesc.FillMode = D3D11_FILL_SOLID;	// Use D3D11_FILL_WIREFRAME to only draw the sides
	rasterizerStateDesc.CullMode = D3D11_CULL_BACK;			// Use D3D11_CULL_NONE to draw on both sides

	ThrowIfFailed(m_d3dDevice->CreateRasterizerState(&rasterizerStateDesc, m_d3dRasterizerState.GetAddressOf()));

	m_d3dDeviceContext->RSSetState(m_d3dRasterizerState.Get());

	/* Converting from regular color format (0-255 rgba to 0-1) */

	m_f4ClearColor[Color::R] = 0.0f;	// r / 255 = DirectX Color (Clamped between 0 and 1)
	m_f4ClearColor[Color::G] = 0.0f;	// g / 255 = DirectX Color (Clamped between 0 and 1)
	m_f4ClearColor[Color::B] = 0.2f;	// b / 255 = DirectX Color (Clamped between 0 and 1)
	m_f4ClearColor[Color::A] = 1.0f;	// a (Clamped between 0 and 1)

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
	// Clear depth and stencil buffers

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

void D3D11Application::SwapBuffers()
{
	// Step11: Present the back buffer to the screen, send the front buffer to the back for the next frame
	m_dxgiSwapChain->Present(
		static_cast<INT>(gfx.VsyncEnabled()), 
		0);
}

void D3D11Application::Clean()
{
	gfx.Clean();
	
	m_d3dDepthStencilView.Reset();
	m_d3dBackBuffer.Reset();
	m_d3dRasterizerState.Reset();
	m_d3dDevice.Reset();
	m_d3dDeviceContext.Reset();
	m_dxgiSwapChain.Reset();
}

D3D11Application::~D3D11Application()
{
}

D3D11Application::D3D11Application()
{
}