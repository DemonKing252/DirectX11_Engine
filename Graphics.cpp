#include "Graphics.h"
#include "Camera.h"
#include "Window.h"
#include "D3D11Application.h"
Graphics::Graphics()
{
	// Zero out all out our buffers and layouts
	ZeroMemory(m_d3dVSConstantBuffer.GetAddressOf(), sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(m_d3dPSConstantBuffer.GetAddressOf(), sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(m_d3dVertexBuffer.GetAddressOf(), sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(m_d3dIndexBuffer.GetAddressOf(), sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(m_d3dVertexShader.GetAddressOf(), sizeof(ID3D11VertexShader));
	ZeroMemory(m_d3dPixelShader.GetAddressOf(), sizeof(ID3D11PixelShader));
	ZeroMemory(m_d3dInputLayout.GetAddressOf(), sizeof(ID3D11InputLayout));
	ZeroMemory(fence_shaderResource.GetAddressOf(), sizeof(ID3D11ShaderResourceView));
	ZeroMemory(m_d3dSamplerState.GetAddressOf(), sizeof(ID3D11SamplerState));
}

Graphics::~Graphics()
{

}

void Graphics::InitPipeline(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Create our pipeline
	ID3DBlob* vs, *ps;
	ID3DBlob *errorQueue;

	ZeroMemory(&vs, sizeof(ID3DBlob));
	ZeroMemory(&ps, sizeof(ID3DBlob));
	ZeroMemory(&errorQueue, sizeof(ID3DBlob));

	/* Compile the vertex shader */
	{
		D3DCompileFromFile(L"VertexShader.hlsl", 0, 0, "VSMain", "vs_4_0", 0, 0, &vs, &errorQueue);
		if (errorQueue != S_OK)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Vertex Shader Could Not Compile!", 0);
			errorQueue->Release();

			Clean();
			D3D11App::Get().Clean();
			
			exit(EXIT_FAILURE);
		}
	}
	/* Compile the pixel shader */
	{
		D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "PSMain", "ps_4_0", 0, 0, &ps, &errorQueue);
		if (errorQueue != S_OK)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Pixel Shader Could Not Compile!", 0);
			errorQueue->Release();
			
			Clean();
			D3D11App::Get().Clean();
			
			exit(EXIT_FAILURE);
		}
	}

	ThrowIfFailed(device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &m_d3dVertexShader));
	ThrowIfFailed(device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &m_d3dPixelShader));

	deviceContext->VSSetShader(m_d3dVertexShader.Get(), 0, 0);
	deviceContext->PSSetShader(m_d3dPixelShader.Get(), 0, 0);

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

	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"Textures/Fence.png", nullptr, fence_shaderResource.GetAddressOf()));
	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"Textures/redstoneLamp.jpg", nullptr, redstoneLamp_shaderResource.GetAddressOf()));

	// what is a texture filter?
	// read:
	// https://gaming.stackexchange.com/questions/48912/whats-the-difference-between-bilinear-trilinear-and-anisotropic-texture-filte
	
	// sampler description
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT;
	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;

	ThrowIfFailed(device->CreateSamplerState(&samplerDesc, &m_d3dSamplerState));
}

void Graphics::InitGraphics(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// Create our cube

	std::array<Vertex, 24> vertices = 
	{
		//					[	vertex position	   ]					[texture coord]			[ normal ]
		/*0*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f )),
		/*1*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f )),
		/*2*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f )),
		/*3*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, -1.0f, 0.0f )),
				
		/*4*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f  )),
		/*5*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f  )),
		/*6*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f  )),
		/*7*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f  )),
			
		/*8*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f  ), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f )),
		/*9*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f  ), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f )),
		/*10*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f  ), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f )),
		/*11*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f  ), DirectX::XMFLOAT3(-1.0f, 0.0f, 0.0f )),
				
		/*12*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f  ), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f  )),
		/*13*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f  ), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f  )),
		/*14*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f  ), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f  )),
		/*15*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f  ), DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f  )),
				
		/*16*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, +0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f  )),
		/*17*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f  )),
		/*18*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, +0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f  )),
		/*19*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, +0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f  )),
				
		/*20*/	Vertex(DirectX::XMFLOAT3(-0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f )),
		/*21*/	Vertex(DirectX::XMFLOAT3(+0.5f, -0.5f, -0.5f ), DirectX::XMFLOAT2( 1.0f, 0.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f )),
		/*22*/	Vertex(DirectX::XMFLOAT3(+0.5f, +0.5f, -0.5f ), DirectX::XMFLOAT2( 1.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f )),
		/*23*/	Vertex(DirectX::XMFLOAT3(-0.5f, +0.5f, -0.5f ), DirectX::XMFLOAT2( 0.0f, 1.0f  ), DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f )),
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

	D3DUtil::Get().Upload(device, m_d3dVertexBuffer, BufferType::VERTEX_BUFFER, sizeof(Vertex) * std::size(vertices), (void*)(&vertices));
	D3DUtil::Get().Upload(device, m_d3dIndexBuffer, BufferType::INDEX_BUFFER, sizeof(UINT) * ARRAYSIZE(indices), (void*)(&indices));

	m_PSConstBuffer.pointLights[0].Position = { 0.0f, 0.0f, 0.0f };
	m_PSConstBuffer.pointLights[0].Strength = { 16.0f, 16.0f, 16.0f };
	m_PSConstBuffer.pointLights[0].SpecularStrength = 1.0f;
	m_PSConstBuffer.pointLights[0].FallOffStart = 0.1f;
	m_PSConstBuffer.pointLights[0].FallOffEnd = 8.0f;

	Projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), // Field of view in radians
		800.0f / 600.0f, // Screen aspect
		0.1f,			// Minimum vocal distance
		300.0f);		// Maximum vocal distance

	m_VSConstBuffer.Proj = DirectX::XMMatrixTranspose(Projection);

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
	float blendFactor[4];

	blendFactor[0] = 0.0f;
	blendFactor[1] = 0.0f;
	blendFactor[2] = 0.0f;
	blendFactor[3] = 0.0f;
		
	deviceContext->OMSetBlendState(this->m_d3dBlendState.Get(), blendFactor, 0xffffffff);

}

void Graphics::Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	// why is this so low? I've been too lazy to add a feature to control the framerate
	mCntr += 0.00125f;
	
	// view matrix (left handed coordinate system)
	View = DirectX::XMMatrixLookAtLH(D3DUtil::Get().m_EyePos,
										DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f),
										DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f));

	DirectX::XMStoreFloat4(&m_PSConstBuffer.EyeWorldSpace, D3DUtil::Get().m_EyePos);

	rotAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	m_VSConstBuffer.View = DirectX::XMMatrixTranspose(View);
}

void Graphics::Draw(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	UINT offset = 0;
	UINT stride = sizeof(Vertex);

	// bind vertex/index buffers, and set our primitive type
	deviceContext->IASetVertexBuffers(0, 1, m_d3dVertexBuffer.GetAddressOf(), &stride, &offset);
	deviceContext->IASetIndexBuffer(m_d3dIndexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set our texture sampler state and texture itself to be active on the pipeline
	deviceContext->PSSetSamplers(0, 1, m_d3dSamplerState.GetAddressOf());
	DirectX::XMFLOAT3 cubeTranslate[10] = 
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

	for (int i = 0; i < ARRAYSIZE(cubeTranslate); i++)
	{
		deviceContext->PSSetShaderResources(0, 1, redstoneLamp_shaderResource.GetAddressOf());
		{
			Model = DirectX::XMMatrixIdentity();
			Model =
				DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *	// Scale
				DirectX::XMMatrixRotationAxis(rotAxis, mCntr) *		// Rotate
				DirectX::XMMatrixTranslation(cubeTranslate[i].x, cubeTranslate[i].y, cubeTranslate[i].z);	// Translate

			m_VSConstBuffer.World = DirectX::XMMatrixTranspose(Model * View * Projection);
			m_VSConstBuffer.Model = DirectX::XMMatrixTranspose(Model);
			UpdateConstants(device, deviceContext);

			deviceContext->DrawIndexed(36, 0, 0);
		}
		deviceContext->PSSetShaderResources(0, 1, fence_shaderResource.GetAddressOf());
		{
			Model = DirectX::XMMatrixIdentity();
			Model =
				DirectX::XMMatrixScaling(1.2f, 1.2f, 1.2f) *	// Scale
				DirectX::XMMatrixRotationAxis(rotAxis, mCntr) *		// Rotate
				DirectX::XMMatrixTranslation(cubeTranslate[i].x, cubeTranslate[i].y, cubeTranslate[i].z);	// Translate

			// Global transform matrix
			m_VSConstBuffer.World = DirectX::XMMatrixTranspose(Model * View * Projection);
			m_VSConstBuffer.Model = DirectX::XMMatrixTranspose(Model);
			UpdateConstants(device, deviceContext);

			deviceContext->DrawIndexed(36, 0, 0);
		}
	}
}

void Graphics::UpdateConstants(ID3D11Device * device, ID3D11DeviceContext * deviceContext)
{
	if (m_d3dVSConstantBuffer != nullptr)
		m_d3dVSConstantBuffer.Reset();

	if (m_d3dPSConstantBuffer != nullptr)
		m_d3dPSConstantBuffer.Reset();

	m_VSConstBuffer.Upload(device, deviceContext, m_d3dVSConstantBuffer, Shader::VS, sizeof(D3D11_VS_CONSTANT_BUFFER), (void*)(&m_VSConstBuffer));
	m_PSConstBuffer.Upload(device, deviceContext, m_d3dPSConstantBuffer, Shader::PS, sizeof(D3D11_PS_CONSTANT_BUFFER), (void*)(&m_PSConstBuffer));
}

void Graphics::Clean()
{
	fence_shaderResource.Reset();
	m_d3dSamplerState.Reset();
	
	m_d3dVertexBuffer.Reset();
	m_d3dIndexBuffer.Reset();
	
	m_d3dInputLayout.Reset();
	m_d3dVertexShader.Reset();
	m_d3dPixelShader.Reset();
	
	m_d3dVSConstantBuffer.Reset();
	m_d3dPSConstantBuffer.Reset();
}