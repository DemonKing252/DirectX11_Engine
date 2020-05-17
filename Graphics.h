#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <array>
#include "D3DCommon.h"
#include "includes/WICTextureLoader.h"

// DirectX Libraries 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
typedef class Graphics
{
public:
	Graphics(Graphics* other) = delete;
	Graphics();
	~Graphics();

	void InitPipeline(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void InitGraphics(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Update(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Draw(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void UpdateConstants(ID3D11Device* device, ID3D11DeviceContext* deviceContext);
	void Clean();
private:
	float mCntr = 0.0f;

	// shader info
	ID3D11VertexShader* m_d3dVertexShader;
	ID3D11PixelShader* m_d3dPixelShader;

	// buffers
	ID3D11Buffer* m_d3dVertexBuffer;
	ID3D11Buffer* m_d3dIndexBuffer;
	ID3D11Buffer* m_d3dVSConstantBuffer;
	ID3D11Buffer* m_d3dPSConstantBuffer;

	// input layout and constant buffer instance
	ID3D11InputLayout* m_d3dInputLayout;

	D3D11_VS_CONSTANT_BUFFER VSConstantBuffer;
	D3D11_PS_CONSTANT_BUFFER PSConstantBuffer;

	// texturing
	ID3D11ShaderResourceView* brick_shaderResource;
	ID3D11SamplerState* m_d3dSamplerState;
	
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;

	DirectX::XMVECTOR rotAxis;

} Graphics, GFX;

