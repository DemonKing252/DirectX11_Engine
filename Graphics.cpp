#include "Graphics.h"

Graphics::Graphics()
{
	// Zero out all out our buffers and layouts
	ZeroMemory(&m_d3dVSConstantBuffer, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_d3dPSConstantBuffer, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_d3dVertexBuffer, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_d3dIndexBuffer, sizeof(D3D11_BUFFER_DESC));
	ZeroMemory(&m_d3dVertexShader, sizeof(ID3D11VertexShader));
	ZeroMemory(&m_d3dPixelShader, sizeof(ID3D11PixelShader));
	ZeroMemory(&m_d3dInputLayout, sizeof(ID3D11InputLayout));
	ZeroMemory(&brick_shaderResource, sizeof(ID3D11ShaderResourceView));
	ZeroMemory(&m_d3dSamplerState, sizeof(ID3D11SamplerState));
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
		if (errorQueue != nullptr)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Vertex Shader Could Not Compile!", 0);
			exit(EXIT_FAILURE);
		}
	}
	/* Compile the pixel shader */
	{
		D3DCompileFromFile(L"PixelShader.hlsl", 0, 0, "PSMain", "ps_4_0", 0, 0, &ps, &errorQueue);
		if (errorQueue != nullptr)
		{
			/* Something went wrong, create a win32 message box and print the error log */
			MessageBox(0, (CHAR*)errorQueue->GetBufferPointer(), "Pixel Shader Could Not Compile!", 0);
			exit(EXIT_FAILURE);
		}
	}

	ThrowIfFailed(device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &m_d3dVertexShader));
	ThrowIfFailed(device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &m_d3dPixelShader));

	deviceContext->VSSetShader(m_d3dVertexShader, 0, 0);
	deviceContext->PSSetShader(m_d3dPixelShader, 0, 0);

	// Our layout is 20 bytes in total. Multiply that by 8 and we get our TOTAL
	// size of our vertex buffer which is 160 bytes (0.16 MB)
	D3D11_INPUT_ELEMENT_DESC inputLayout[] =
	{
		// layout 1 -> 3D vector representing vertex positions
		// 12 bytes in V-RAM
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		
		// layout 2 -> 2D vector representing vertex texture coordinates
		// 8 bytes in V-RAM
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ThrowIfFailed(device->CreateInputLayout(inputLayout, ARRAYSIZE(inputLayout), vs->GetBufferPointer(), vs->GetBufferSize(), &m_d3dInputLayout));
	deviceContext->IASetInputLayout(m_d3dInputLayout);

	ThrowIfFailed(DirectX::CreateWICTextureFromFile(device, L"Textures/redstoneLamp.jpg", nullptr, &brick_shaderResource));

	// what is a texture filter?
	// read:
	// https://gaming.stackexchange.com/questions/48912/whats-the-difference-between-bilinear-trilinear-and-anisotropic-texture-filte
	
	// sampler description
	D3D11_SAMPLER_DESC samplerDesc;
	ZeroMemory(&samplerDesc, sizeof(D3D11_SAMPLER_DESC));

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_LINEAR_MIP_POINT;
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
		//					[	vertex position	   ]					[texture coord] 
		/*0*/	DirectX::XMFLOAT3{-0.5f, -0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f  },
		/*1*/	DirectX::XMFLOAT3{+0.5f, -0.5f, -0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f  },
		/*2*/	DirectX::XMFLOAT3{+0.5f, -0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f  },
		/*3*/	DirectX::XMFLOAT3{-0.5f, -0.5f, +0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f  },
			
		/*4*/	DirectX::XMFLOAT3{-0.5f, +0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f  },
		/*5*/	DirectX::XMFLOAT3{+0.5f, +0.5f, -0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f  },
		/*6*/	DirectX::XMFLOAT3{+0.5f, +0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f  },
		/*7*/	DirectX::XMFLOAT3{-0.5f, +0.5f, +0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f  },
				
		/*8*/	DirectX::XMFLOAT3{-0.5f, -0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f  },
		/*9*/	DirectX::XMFLOAT3{-0.5f, -0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f  },
		/*10*/	DirectX::XMFLOAT3{-0.5f, +0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f  },
		/*11*/	DirectX::XMFLOAT3{-0.5f, +0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f  },
			
		/*12*/	DirectX::XMFLOAT3{+0.5f, -0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f  },
		/*13*/	DirectX::XMFLOAT3{+0.5f, -0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f  },
		/*14*/	DirectX::XMFLOAT3{+0.5f, +0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f  },
		/*15*/	DirectX::XMFLOAT3{+0.5f, +0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f  },
				
		/*16*/	DirectX::XMFLOAT3{-0.5f, -0.5f, +0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f  },
		/*17*/	DirectX::XMFLOAT3{+0.5f, -0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f  },
		/*18*/	DirectX::XMFLOAT3{+0.5f, +0.5f, +0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f  },
		/*19*/	DirectX::XMFLOAT3{-0.5f, +0.5f, +0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f  },
			
		/*20*/	DirectX::XMFLOAT3{-0.5f, -0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 0.0f  },
		/*21*/	DirectX::XMFLOAT3{+0.5f, -0.5f, -0.5f }, DirectX::XMFLOAT2{ 1.0f, 0.0f  },
		/*22*/	DirectX::XMFLOAT3{+0.5f, +0.5f, -0.5f }, DirectX::XMFLOAT2{ 1.0f, 1.0f  },
		/*23*/	DirectX::XMFLOAT3{-0.5f, +0.5f, -0.5f }, DirectX::XMFLOAT2{ 0.0f, 1.0f  },
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

	// vertex buffer
	D3D11_BUFFER_DESC vBufferDesc;
	ZeroMemory(&vBufferDesc, sizeof(D3D11_BUFFER_DESC));

	vBufferDesc.Usage = D3D11_USAGE_DYNAMIC;						// write access access by CPU and GPU
	vBufferDesc.ByteWidth = sizeof(Vertex) * std::size(vertices);   // size is the VERTEX struct * 3
	vBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;				// use as a vertex buffer
	vBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;			// allow CPU to write in buffer

	D3D11_SUBRESOURCE_DATA vSubResource;
	ZeroMemory(&vSubResource, sizeof(D3D11_SUBRESOURCE_DATA));

	vSubResource.pSysMem = &vertices;

	ThrowIfFailed(device->CreateBuffer(&vBufferDesc, &vSubResource, &m_d3dVertexBuffer));

	// index buffer

	D3D11_BUFFER_DESC iBufferDesc;
	ZeroMemory(&iBufferDesc, sizeof(D3D11_BUFFER_DESC));

	iBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	iBufferDesc.ByteWidth = sizeof(UINT) * ARRAYSIZE(indices);
	iBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	iBufferDesc.CPUAccessFlags = 0;

	D3D11_SUBRESOURCE_DATA iSubResource;
	ZeroMemory(&iSubResource, sizeof(D3D11_SUBRESOURCE_DATA));

	iSubResource.pSysMem = &indices;

	ThrowIfFailed(device->CreateBuffer(&iBufferDesc, &iSubResource, &m_d3dIndexBuffer));
}

void Graphics::Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	if (m_d3dVSConstantBuffer != nullptr)
		m_d3dVSConstantBuffer->Release();

	if (m_d3dPSConstantBuffer != nullptr)
		m_d3dPSConstantBuffer->Release();


	// why is this so low? I've been too lazy to add a feature to control the framerate
	zaxis_angle += 0.00125f;
	c_ps_Buffer.color.x = 1.0f - (0.5f * (cos(zaxis_angle)) + 0.5f);
	c_ps_Buffer.color.y = (0.5f * (cos(zaxis_angle)) + 0.5f);
	
	//OutputDebugString((LPCSTR)std::to_string(c_ps_Buffer.color.x));

	c_vs_Buffer.World = DirectX::XMMatrixIdentity();

	// eye position in world space
	DirectX::XMVECTOR EyePosition = DirectX::XMVectorSet(0.0f, 1.0f, -3.0f, 0.0f);
	
	// focus position in world space (where the camera is looking at)
	DirectX::XMVECTOR EyeFocus = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// up vector (represents the orientation of the camera). 
	// [ 0, -1, 0 ] to make the camera look upside down
	DirectX::XMVECTOR Up = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// view matrix (left handed coordinate system)
	DirectX::XMMATRIX view = DirectX::XMMatrixLookAtLH(EyePosition, EyeFocus, Up);
	
	// projection matrix (45 degrees left/right, with an aspect ration of 1 1/3 (screenWidth / screenHeight)
	DirectX::XMMATRIX projection = DirectX::XMMatrixPerspectiveFovLH(DirectX::XMConvertToRadians(45.0f), 800.0f/600.0f, 0.1f, 300.0f);

	// what axis should our object rotate on?
	DirectX::XMVECTOR rotAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 1.0f);

	// finally, polulate a model matrix with transform information 
	// must go in scale-rotate-translate order
	DirectX::XMMATRIX Model =
		DirectX::XMMatrixScaling(1.0f, 1.0f, 1.0f) *			// Scale
		DirectX::XMMatrixRotationAxis(rotAxis, zaxis_angle) *   // Rotate
		DirectX::XMMatrixTranslation(0.0f, 0.0f, 0.0f);			// Translate
	
	// multiply our 3 matricies to get the world matrix
	c_vs_Buffer.World = Model * view * projection;

	// transpose (our D3D11 vertex shader expects to have COLUMN majors!!!)
	c_vs_Buffer.World = DirectX::XMMatrixTranspose(c_vs_Buffer.World);

	// Update constant buffer in the vertex shader
	// -----------------------------------------------------------------
	D3D11_BUFFER_DESC c_vs_BufferDesc;
	ZeroMemory(&c_vs_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	c_vs_BufferDesc.ByteWidth = sizeof(D3D11_VS_CONSTANT_BUFFER);
	c_vs_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	c_vs_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	c_vs_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	D3D11_SUBRESOURCE_DATA cSubResource;
	ZeroMemory(&cSubResource, sizeof(D3D11_SUBRESOURCE_DATA));

	cSubResource.pSysMem = &c_vs_Buffer;

	ThrowIfFailed(device->CreateBuffer(&c_vs_BufferDesc, &cSubResource, &m_d3dVSConstantBuffer));
	deviceContext->VSSetConstantBuffers(0, 1, &m_d3dVSConstantBuffer);
	// -----------------------------------------------------------------


	// Update constant buffer in the pixel shader
	// -----------------------------------------------------------------

	D3D11_SUBRESOURCE_DATA c_ps_SubResource;
	ZeroMemory(&cSubResource, sizeof(D3D11_SUBRESOURCE_DATA));

	cSubResource.pSysMem = &c_ps_Buffer;

	ThrowIfFailed(device->CreateBuffer(&c_vs_BufferDesc, &cSubResource, &m_d3dPSConstantBuffer));
	deviceContext->PSSetConstantBuffers(0, 1, &m_d3dPSConstantBuffer);
	// -----------------------------------------------------------------
}

void Graphics::Draw(ID3D11DeviceContext* deviceContext)
{
	UINT offset = 0;
	UINT stride = sizeof(Vertex);

	// bind vertex/index buffers, set our primitive type
	deviceContext->IASetVertexBuffers(0, 1, &m_d3dVertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(m_d3dIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// set our texture sampler state and texture itself to be active on the pipeline
	deviceContext->PSSetSamplers(0, 1, &m_d3dSamplerState);
	deviceContext->PSSetShaderResources(0, 1, &brick_shaderResource);

	// draw call
	deviceContext->DrawIndexed(36, 0, 0);
}

void Graphics::Clean()
{
	m_d3dVertexBuffer->Release();
	m_d3dIndexBuffer->Release();
	m_d3dInputLayout->Release();
	m_d3dVertexShader->Release();
	m_d3dPixelShader->Release();
	m_d3dVSConstantBuffer->Release();
	m_d3dPSConstantBuffer->Release();
}
