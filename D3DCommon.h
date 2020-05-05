#pragma once
#include <DirectXMath.h>
#include <stdexcept>
#include <Windows.h>
#include <iostream>
#include <string>
struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};
struct D3D11_VS_CONSTANT_BUFFER
{
	DirectX::XMMATRIX World;
};
struct D3D11_PS_CONSTANT_BUFFER
{
	DirectX::XMFLOAT3 color = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
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


private:
	static D3DUtil instance;
};

