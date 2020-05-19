#pragma once
#include <d3d11.h>
#include <d3dcompiler.h>
#include <wrl.h>
#include <DirectXMath.h>
#include "D3DCommon.h"
class D3DUploadBuffer
{
public:
	void Upload(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, Shader shader, UINT byteSize, void* data);
};

class D3D11_VS_CONSTANT_BUFFER : public D3DUploadBuffer
{
public:
	DirectX::XMMATRIX World;

	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;

	D3D11_VS_CONSTANT_BUFFER() {}
};
class D3D11_PS_CONSTANT_BUFFER : public D3DUploadBuffer
{
public:
	DirectX::XMFLOAT4 EyeWorldSpace;

	PointLight pointLights[MaxLights];

	D3D11_PS_CONSTANT_BUFFER() {}
};

