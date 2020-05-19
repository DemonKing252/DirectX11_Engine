#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <array>
#include "D3DCommon.h"
#include "Vertex.h"
#include "includes/WICTextureLoader.h"
#include <memory>
#include "Window.h"
#include "D3DUtil.h"
#include "ConstantBuffer.h"
// DirectX Libraries 
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "DirectXTK.lib")
typedef class Graphics
{
public:
	// Delete the copy constructor. Only one instance of graphics should be created!
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
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dIndexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dVSConstantBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_d3dPSConstantBuffer;

	// input layout and constant buffer instance
	ID3D11InputLayout* m_d3dInputLayout;

	D3D11_VS_CONSTANT_BUFFER m_VSConstBuffer;
	D3D11_PS_CONSTANT_BUFFER m_PSConstBuffer;
	// texturing
	ID3D11ShaderResourceView* brick_shaderResource;
	ID3D11SamplerState* m_d3dSamplerState;
	
	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Projection;

	DirectX::XMVECTOR rotAxis;
} Graphics, GFX;