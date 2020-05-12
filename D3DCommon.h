#pragma once
#include <DirectXMath.h>
#include <stdexcept>
#include <Windows.h>
#include <iostream>
#include <string>
#define MaxLights 2
struct PointLight
{
	DirectX::XMFLOAT3 Position;
	FLOAT SpecularStrength;

	DirectX::XMFLOAT3 Strength;
	FLOAT FallOffStart;

	DirectX::XMFLOAT3 Direction;
	FLOAT FallOffEnd;

};

struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
	DirectX::XMFLOAT3 Normal;
};
struct D3D11_VS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX World;

	DirectX::XMMATRIX Model;
	DirectX::XMMATRIX View;
	DirectX::XMMATRIX Proj;

};
struct D3D11_PS_CONSTANT_BUFFER
{
	DirectX::XMFLOAT4 Color = DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMFLOAT4 EyeWorldSpace;

	PointLight pointLights[MaxLights];
};

class DirectXException
{
public:
    DirectXException(HRESULT hr);
    std::string HrToString(HRESULT hr)const;
};
#define ThrowIfFailed(hr) if (FAILED(hr)) { throw DirectXException(hr); }
class D3DUtil
{
public:
	DirectX::XMVECTOR m_EyePos = DirectX::XMVectorSet(0.0f, 0.0f, 3.0f, 0.0f);
	BOOL leftPressed = false;
	BOOL rightPressed = false;
	DirectX::XMFLOAT2 m_mousePos = DirectX::XMFLOAT2(0, 0);

	D3DUtil(D3DUtil* other) = delete;
	D3DUtil() = default;

	static D3DUtil& Get()
	{
		return instance;
	}
	DirectX::XMFLOAT3 Light = DirectX::XMFLOAT3(2.0f, 0.0f, 0.0f);


private:
	static D3DUtil instance;
};

