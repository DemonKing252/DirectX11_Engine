#include "Graphics.h"
#include "Camera.h"
#include "Window.h"
#include "D3D11Application.h"
#include "ConstantBuffer.h"
Graphics::Graphics()
{
	// Step12: Zero out our vertex buffer, vertex shader, pixel shader, and input layout
	// Zero out all out our buffers and layouts
	
	ZeroMemory(m_d3dVSConstantBuffer.GetAddressOf(), sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(m_d3dPSConstantBuffer.GetAddressOf(), sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(m_d3dVertexShader.GetAddressOf(), sizeof(ID3D11VertexShader));
	ZeroMemory(m_d3dPixelShaderNoIllumination.GetAddressOf(), sizeof(ID3D11PixelShader));
	ZeroMemory(m_d3dInputLayout.GetAddressOf(), sizeof(ID3D11InputLayout));
	ZeroMemory(m_shaderResources[ShaderResource::Fence].GetAddressOf(), sizeof(ID3D11ShaderResourceView));
	ZeroMemory(m_shaderResources[ShaderResource::RedstoneLamp].GetAddressOf(), sizeof(ID3D11ShaderResourceView));
	ZeroMemory(m_shaderResources[ShaderResource::StoneBrick].GetAddressOf(), sizeof(ID3D11ShaderResourceView));
	ZeroMemory(m_d3dSamplerState.GetAddressOf(), sizeof(ID3D11SamplerState));

	m_PSConstBuffer = new PSConstBuffer();
	m_VSConstBuffer = new VSConstBuffer();
}

Graphics::~Graphics()
{
}

void Graphics::InitPipeline(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Step 13: Compile and attach the vertex and pixel shaders to our pipeline
	// Create our pipeline
	ID3DBlob* vs, *ps1, *ps2;
	ID3DBlob *errorQueue;

	ZeroMemory(&vs, sizeof(ID3DBlob));
	ZeroMemory(&ps1, sizeof(ID3DBlob));
	ZeroMemory(&ps2, sizeof(ID3DBlob));
	ZeroMemory(&errorQueue, sizeof(ID3DBlob));

	/* Compile the vertex shader */
	{
		D3DCompileFromFile(L"VS.hlsl", 0, 0, "VSMain", "vs_4_0", 0, 0, &vs, &errorQueue);
		if (errorQueue != S_OK)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Vertex Shader Could Not Compile!", 0);
			errorQueue->Release();

			Clean();
			D3D11Engine::Get().Clean();
			
			exit(EXIT_FAILURE);
		}
		ThrowIfFailed(device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &m_d3dVertexShader));
	}
	
	/* Compile pixel shader #1 */
	{
		D3DCompileFromFile(L"DefaultPS.hlsl", 0, 0, "PSMain", "ps_4_0", 0, 0, &ps1, &errorQueue);
		if (errorQueue != S_OK)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Pixel Shader Could Not Compile!", 0);
			errorQueue->Release();
			
			Clean();
			D3D11Engine::Get().Clean();
			
			exit(EXIT_FAILURE);
		}
		ThrowIfFailed(device->CreatePixelShader(ps1->GetBufferPointer(), ps1->GetBufferSize(), NULL, &m_d3dPixelShaderDefault));
	}
	/* Compile pixel shader #2 */
	{
		D3DCompileFromFile(L"AbsenseIlluminationPS.hlsl", 0, 0, "PSMain", "ps_4_0", 0, 0, &ps2, &errorQueue);
		if (errorQueue != S_OK)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Pixel Shader Could Not Compile!", 0);
			errorQueue->Release();

			Clean();
			D3D11Engine::Get().Clean();

			exit(EXIT_FAILURE);
		}
		ThrowIfFailed(device->CreatePixelShader(ps2->GetBufferPointer(), ps2->GetBufferSize(), NULL, &m_d3dPixelShaderNoIllumination));
	}
	deviceContext->VSSetShader(m_d3dVertexShader.Get(), 0, 0);

	// Step14: Create an input layout that our vertex shader will use
	// Our layout is 20 bytes in total. Multiply that by 8 and we get our TOTAL
	// size of our vertex buffer which is 160 bytes (0.16 MB)
	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		// layout 1 -> 3D vector representing vertex positions
		// 12 bytes in V-RAM --> [12 bytes in vertex]
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
		// layout 2 -> 2D vector representing vertex texture coordinates
		// 8 bytes in V-RAM --> [20 bytes in vertex]
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },

		// layout 3 -> 3D vector representing vertex texture normals
		// 12 bytes in V-RAM --> [32 bytes in vertex]
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ThrowIfFailed(device->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout), vs->GetBufferPointer(), vs->GetBufferSize(), m_d3dInputLayout.GetAddressOf()));
	deviceContext->IASetInputLayout(m_d3dInputLayout.Get());

	// Fence texture
	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"Textures/Fence.png", nullptr, m_shaderResources[ShaderResource::Fence].GetAddressOf()));
	
	// Redstone lamp texture
	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"Textures/redstoneLamp.jpg", nullptr, m_shaderResources[ShaderResource::RedstoneLamp].GetAddressOf()));

	// Brick texture
	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"Textures/StoneBrick.jpg", nullptr, m_shaderResources[ShaderResource::StoneBrick].GetAddressOf()));


	// Read on D3D11 Sampler States:
	// https://gaming.stackexchange.com/questions/48912/whats-the-difference-between-bilinear-trilinear-and-anisotropic-texture-filte
	
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(device->CreateSamplerState(&samplerDesc, &m_d3dSamplerState));
}

void Graphics::InitGraphics(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Step15: Create a vertex buffer blob, and fill it with our data.
	// Making sure we use the same vertex setup as our input layout!

	Vertex vertices[] = 
	{
		//								[  vertex position  ]					[texture coord]				   [	  normal	  ]
		/*0*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*1*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*2*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
		/*3*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f)),
				
		/*4*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*5*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*6*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
		/*7*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f)),
			
		/*8*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*9*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*10*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
		/*11*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f)),
				
		/*12*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*13*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*14*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
		/*15*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f)),
				
		/*16*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*17*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*18*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
		/*19*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f)),
				
		/*20*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*21*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 0.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*22*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f), DirectX::XMFLOAT2(1.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
		/*23*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f), DirectX::XMFLOAT2(0.0f, 1.0f), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f)),
	};
	UINT indices[36]
	{
		// bottom
		0, 1, 2,
		2, 3, 0,

		// top
		6, 5, 4,
		4, 7, 6,

		// left
		8, 9, 10,
		10, 11, 8,

		// right
		14, 13, 12,
		12, 15, 14,

		// back 
		16, 17, 18,
		18, 19, 16,

		// front
		22, 21, 20,
		20, 23, 22

	};
	
	cubeSubMesh = std::make_shared<SubMeshGeometry>();

	cubeSubMesh->IndexCount = ARRAYSIZE(indices);
	cubeSubMesh->VertexCount = ARRAYSIZE(vertices);

	// Add a vertex buffer component to our cube
	cubeSubMesh->AddComponent<VertexBufferComponent<Vertex>>();
	cubeSubMesh->GetComponent<VertexBufferComponent<Vertex>>().ZeroMem();
	cubeSubMesh->GetComponent<VertexBufferComponent<Vertex>>().Initialize(device, vertices, ARRAYSIZE(vertices));

	// Add a index buffer component to our cube
	cubeSubMesh->AddComponent<IndexBufferComponent<UINT>>();
	cubeSubMesh->GetComponent<IndexBufferComponent<UINT>>().ZeroMem();
	cubeSubMesh->GetComponent<IndexBufferComponent<UINT>>().Initialize(device, indices, ARRAYSIZE(indices));
		
	DirectX::XMFLOAT3 temp_positions[10] =
	{
		{ +2.0f, 0.0f, 0.0f },
		{ -2.0f, 0.0f, 0.0f },
		{ +4.0f, 0.0f, 0.0f },
		{ -4.0f, 0.0f, 0.0f },
		{ +6.0f, 0.0f, 0.0f },
		{ -6.0f, 0.0f, 0.0f },
		{ +8.0f, 0.0f, 0.0f },
		{ -8.0f, 0.0f, 0.0f },
		{ +10.0f, 0.0f, 0.0f },
		{ -10.0f, 0.0f, 0.0f },
	};
	for (int i = 0; i < ARRAYSIZE(temp_positions); i++)
	{
		m_vRenderItems.push_back(std::make_shared<RenderItem>());
		m_vRenderItems.back()->AddComponent<TransformComponent>(); 

		m_vRenderItems.back()->GetComponent<TransformComponent>().SetTranslation(temp_positions[i]);
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetScaling(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetRotationAxis(DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f));
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetAngle(0.0f);
	
		DirectX::XMMATRIX Model =
				DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// Scale
				DirectX::XMMatrixRotationY(0.0f) *		// Rotate
				DirectX::XMMatrixTranslation(temp_positions[i].x, temp_positions[i].y, temp_positions[i].z);	// Translate

		m_vRenderItems.back()->GetComponent<TransformComponent>().SetModelMatrix(Model);
		m_vRenderItems.back()->shaderResource = ShaderResource::RedstoneLamp;
		m_vRenderItems.back()->m_bDoesRotate = false;
		m_vRenderItems.back()->m_bIsLight = false;
		m_vRenderItems.back()->pixelShader = PixelShader::Default;
	}
	// Platform
	{
		m_vRenderItems.push_back(std::make_shared<RenderItem>());
		m_vRenderItems.back()->AddComponent<TransformComponent>();

		m_vRenderItems.back()->GetComponent<TransformComponent>().SetTranslation({ 0.0f, -2.0f, 0.0f });
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetScaling({ 20.0f, 1.0f, 20.0f });
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetRotationAxis({ 0.0f, 1.0f, 0.0f});
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetAngle(0.0f);

		DirectX::XMMATRIX Model =
			DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// Scale
			DirectX::XMMatrixTranslation(0.0f, -2.0f, 0.0f);	// Translate

		m_vRenderItems.back()->GetComponent<TransformComponent>().SetModelMatrix(Model);
		m_vRenderItems.back()->shaderResource = ShaderResource::StoneBrick;
		m_vRenderItems.back()->m_bDoesRotate = false;
		m_vRenderItems.back()->m_bIsLight = false;
		m_vRenderItems.back()->pixelShader = PixelShader::Default;
	}
	// Lights
	for (int i = 0; i < PointLightCount; i++)
	{
		m_vRenderItems.push_back(std::make_shared<RenderItem>());
		m_vRenderItems.back()->AddComponent<TransformComponent>();

		m_vRenderItems.back()->GetComponent<TransformComponent>().SetTranslation(m_PSConstBuffer->pointLights[i].Position);
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetScaling({ 1.0f, 1.0f, 1.0f });
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetRotationAxis({ 0.0f, 1.0f, 0.0f });
		m_vRenderItems.back()->GetComponent<TransformComponent>().SetAngle(0.0f);

		DirectX::XMMATRIX Model =
			DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// Scale
			DirectX::XMMatrixTranslation(m_PSConstBuffer->pointLights[i].Position.x,
									     m_PSConstBuffer->pointLights[i].Position.y,
									     m_PSConstBuffer->pointLights[i].Position.z);	// Translate

		m_vRenderItems.back()->GetComponent<TransformComponent>().SetModelMatrix(Model);
		m_vRenderItems.back()->shaderResource = ShaderResource::StoneBrick;
		m_vRenderItems.back()->m_bDoesRotate = false;
		m_vRenderItems.back()->m_bIsLight = true;
		m_vRenderItems.back()->pixelShader = PixelShader::NoIllumination;
	}

	m_PSConstBuffer->pointLights[0].Position = { 0.0f, 0.0f, 5.0f };
	m_PSConstBuffer->pointLights[0].Strength = { 1.0f, 0.6f, 0.0f };
	m_PSConstBuffer->pointLights[0].SpecularStrength = 4.0f;
	m_PSConstBuffer->pointLights[0].FallOffStart = 1.0f;
	m_PSConstBuffer->pointLights[0].FallOffEnd = 8.0f;

	m_PSConstBuffer->pointLights[1].Position = { 0.0f, 0.0f, -5.0f };
	m_PSConstBuffer->pointLights[1].Strength = { 0.0f, 1.0f, 0.0f };
	m_PSConstBuffer->pointLights[1].SpecularStrength = 4.0f;
	m_PSConstBuffer->pointLights[1].FallOffStart = 1.0f;
	m_PSConstBuffer->pointLights[1].FallOffEnd = 8.0f;

	m_4x4Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), // Field of view in radians
		800.0f / 600.0f, // Screen aspect
		0.1f,			 // Minimum vocal distance
		300.0f);		 // Maximum vocal distance

	m_VSConstBuffer->Proj = DirectX::XMMatrixTranspose(m_4x4Projection);

	D3D11_BLEND_DESC blendStateDesc;
	ZeroMemory(&blendStateDesc, sizeof(D3D11_BLEND_DESC));

	blendStateDesc.RenderTarget[0].BlendEnable = TRUE;
	blendStateDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
	
	blendStateDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blendStateDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	
	blendStateDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blendStateDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blendStateDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;

	ThrowIfFailed(device->CreateBlendState(&blendStateDesc, m_d3dBlendState.GetAddressOf()));
	float blendFactor[4] = { 0.0f, 0.0f, 0.0f, 0.0f };
		
	deviceContext->OMSetBlendState(m_d3dBlendState.Get(), blendFactor, 0xffffffff);
}

void Graphics::InitImGui(ID3D11Device * device, ID3D11DeviceContext* deviceContext, std::shared_ptr<Window> window)
{
	// ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	
	assert(ImGui_ImplWin32_Init(window->GetWindow()));
	assert(ImGui_ImplDX11_Init(device, deviceContext));
	ImGui::StyleColorsDark();
}

void Graphics::Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// why is this so low? I've been too lazy to add a feature to control the framerate

	mCntr += 1.0f; // With vsync enabled (locked at 60fps), each cube will rotate 60 degrees per second
	
	// view matrix (left handed coordinate system)
	m_4x4View = DirectX::XMMatrixLookAtLH(D3DUtil::Get().m_EyePos,
										DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
										DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat4(&m_PSConstBuffer->EyeWorldSpace, D3DUtil::Get().m_EyePos);


	m_VSConstBuffer->View = DirectX::XMMatrixTranspose(m_4x4View);
}

void Graphics::Draw(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	assert(cubeSubMesh->HasComponent<VertexBufferComponent<Vertex>>());

	UINT offset = cubeSubMesh->GetComponent<VertexBufferComponent<Vertex>>().GetOffSet();
	UINT stride = cubeSubMesh->GetComponent<VertexBufferComponent<Vertex>>().GetStride();

	// Step16: Attach our vertex buffer and set our desired primitive topology type
	// Read ---> [https://docs.microsoft.com/en-us/windows/win32/api/d3dcommon/ne-d3dcommon-d3d_primitive_topology]

	// bind vertex/index buffers, and set our primitive type
	deviceContext->IASetVertexBuffers(0, 1, cubeSubMesh->GetComponent<VertexBufferComponent<Vertex>>().GetBuffer().GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(cubeSubMesh->GetComponent<IndexBufferComponent<UINT>>().GetBuffer().Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set our texture sampler state and texture itself to be active on the pipeline
	deviceContext->PSSetSamplers(0, 1, m_d3dSamplerState.GetAddressOf());

	// Step17: Draw our shape

	/* Important Notes */
	/******************************************************************
		
		1. Make sure ALL of your buffers you want to use are active on the pipline, either here or earlier.
		
		2. Important that you draw in clockwise rotation! Otherwise the primitives will be drawn from the opposite side
		which is not ideal. WE CAN FIX THIS THOUGH! If you want your primitives to draw in counter-clockwise rotation OR you want them regardless of the 
		order, we can change the rasterizer state settings.

		3. If you're using a constant buffer, make sure its total size 
		is a multiple of *16* bytes in memory. Group your variables from smallest to largest.
		
		Example:
		[ ...............Constant Buffer..................]
		[-------------------------------------------------]
		 | float4 |  | float4 |  | float4x4 | = 24 floats
		 (16 bytes)	 (16 bytes)  ( 64 bytes ) = 96 bytes / 16 = 6.0 <- Good!
		[-------------------------------------------------]
							  
		[ ...............Constant Buffer..................]
		[-------------------------------------------------]
		 | float2 |  | float3 |  | float3x3 | = 14 floats
		 ( 8 bytes)	 (12 bytes)  ( 36 bytes ) = 56 bytes / 16 = 3.5 <- Won't work!
		[-------------------------------------------------]

	******************************************************************/

	// Save on performance! 
	// Important because many of these items use the same shader resource!
	// Save every second you can on rendering time!

	ShaderResource activeResource = ShaderResource::None;
	PixelShader activePixelShader = PixelShader::Undefined;

	deviceContext->PSSetShader(m_d3dPixelShaderNoIllumination.Get(), 0, 0);
	for (int i = 0; i < PointLightCount; i++)
	{
		m_4x4Model =
			DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// Scale
			DirectX::XMMatrixTranslation(m_PSConstBuffer->pointLights[i].Position.x, 
										 m_PSConstBuffer->pointLights[i].Position.y,
										 m_PSConstBuffer->pointLights[i].Position.z);	// Translate
	
		// World transform matrix
		m_VSConstBuffer->World = DirectX::XMMatrixTranspose(m_4x4Model * m_4x4View * m_4x4Projection);
	
		// Local transform matrix
		m_VSConstBuffer->Model = DirectX::XMMatrixTranspose(m_4x4Model);
	
		m_PSConstBuffer->Color = 
		{
			m_PSConstBuffer->pointLights[i].Strength.x,
			m_PSConstBuffer->pointLights[i].Strength.y,
			m_PSConstBuffer->pointLights[i].Strength.z,
			1.0f
		};
		this->UpdateConstants(device, deviceContext);	// Update constant variables in the vertex/pixel shader
		deviceContext->DrawIndexed(cubeSubMesh->IndexCount, 0, 0);		// Draw Call using index buffer
	}
	
	deviceContext->PSSetShader(m_d3dPixelShaderDefault.Get(), 0, 0);
	for (auto ri : m_vRenderItems)
	{
		assert(ri->HasComponent<TransformComponent>());

		if (ri->shaderResource != activeResource)
			deviceContext->PSSetShaderResources(0, 1, m_shaderResources[ri->shaderResource].GetAddressOf());
		{

			m_4x4Model =
				DirectX::XMMatrixScaling(ri->GetComponent<TransformComponent>().GetScaling().x,
					ri->GetComponent<TransformComponent>().GetScaling().y,
					ri->GetComponent<TransformComponent>().GetScaling().z) *

				DirectX::XMMatrixRotationAxis(DirectX::XMLoadFloat3(&ri->GetComponent<TransformComponent>().GetRotationAxis()),
																   	 ri->m_bDoesRotate ? mCntr * DirectX::XM_PI / 180.0f : 0.0f) *

				DirectX::XMMatrixTranslation(ri->GetComponent<TransformComponent>().GetTranslation().x, 
											 ri->GetComponent<TransformComponent>().GetTranslation().y, 
											 ri->GetComponent<TransformComponent>().GetTranslation().z);	// Translate

			// World transform matrix
			m_VSConstBuffer->World = DirectX::XMMatrixTranspose(m_4x4Model * m_4x4View * m_4x4Projection);	

			// Local transform matrix
			m_VSConstBuffer->Model = DirectX::XMMatrixTranspose(m_4x4Model);	
			
			this->UpdateConstants(device, deviceContext);	// Update constant variables in the vertex/pixel shader
			deviceContext->DrawIndexed(cubeSubMesh->IndexCount, 0, 0);		// Draw Call using index buffer
		}
	}

	// ImGui
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGui::Begin("Enviorment Settings");
	ImGui::SetWindowPos(ImVec2(0, 0));
	ImGui::SetWindowSize(ImVec2(365, 272));

	// Determine if we need to update our constant buffers
	
	ImGui::ColorEdit3("Background color", (float*)&D3D11App::Get().m_f4ClearColor); // Edit 3 floats representing a color
	
	ImGui::Text("Light Info");
	ImGui::Separator();
	
	char buffer[20];
	for (int i = 0; i < PointLightCount; i++) 
	{
		float light_color[3] =
		{
			 m_PSConstBuffer->pointLights[i].Strength.x, m_PSConstBuffer->pointLights[i].Strength.y, m_PSConstBuffer->pointLights[i].Strength.z,
		};

		sprintf_s(buffer, "Color #%d", i + 1);
		if (ImGui::SliderFloat3(buffer, light_color, 0.0f, 1.0f, "%.1f"))
		{
			m_PSConstBuffer->pointLights[i].Strength.x = light_color[Color::R];
			m_PSConstBuffer->pointLights[i].Strength.y = light_color[Color::G];
			m_PSConstBuffer->pointLights[i].Strength.z = light_color[Color::B];
			m_bSettings_have_changed = true;
		}
		sprintf_s(buffer, "Range #%d", i + 1);
		if (ImGui::SliderFloat(buffer, &m_PSConstBuffer->pointLights[i].FallOffEnd, 0.1f, 20.0f, "%.1f"))
			m_bSettings_have_changed = true;

		ImGui::Separator();
	}
	ImGui::Checkbox("VSync Enabled", &m_bVsyncEnabled);

	ImGui::NewLine();
	ImGui::Text("DirectX v11.0 Window (%.0fx%.0f)", ImGui::GetWindowSize().x, ImGui::GetWindowSize().y);
	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Text("By: Liam Blake (C) 2020 All Rights Reserved.");

	ImGui::End();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	if (m_bSettings_have_changed) {
		this->UpdateConstants(device, deviceContext);
		m_bSettings_have_changed = false;
	}
}

void Graphics::UpdateConstants(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	if (m_d3dVSConstantBuffer != nullptr)
		m_d3dVSConstantBuffer.Reset();

	if (m_d3dPSConstantBuffer != nullptr)
		m_d3dPSConstantBuffer.Reset();

	m_VSConstBuffer->Upload(device, 
							deviceContext, 
							m_d3dVSConstantBuffer, 
							Shader::VS, 
							sizeof(VSConstBuffer), 
							reinterpret_cast<LPVOID>(m_VSConstBuffer));
							/* Convert from its class instance form to a void pointer of memory */

	m_PSConstBuffer->Upload(device, 
							deviceContext, 
							m_d3dPSConstantBuffer, 
							Shader::PS, 
							sizeof(PSConstBuffer), 
							reinterpret_cast<LPVOID>(m_PSConstBuffer));
							/* Convert from its class instance form to a void pointer of memory */
}

void Graphics::Clean()
{
	delete m_VSConstBuffer;
	delete m_PSConstBuffer;

	for (int i = 0; i < 3; i++)
	{
		m_shaderResources[i].Reset();
	}

	//m_shaderResources.Reset();
	//m_d3dSamplerState.Reset();
	
	cubeSubMesh->GetComponent<VertexBufferComponent<Vertex>>().GetBuffer().Reset();
	cubeSubMesh->GetComponent<IndexBufferComponent<UINT>>().GetBuffer().Reset();
	
	m_d3dInputLayout.Reset();
	m_d3dVertexShader.Reset();
	m_d3dPixelShaderNoIllumination.Reset();
	
	m_d3dVSConstantBuffer.Reset();
	m_d3dPSConstantBuffer.Reset();
}

bool Graphics::VsyncEnabled() const
{
	return m_bVsyncEnabled;
}
