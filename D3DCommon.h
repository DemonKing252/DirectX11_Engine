#pragma once
#include <DirectXMath.h>
#include <stdexcept>
#include <Windows.h>
#include <iostream>
struct Vertex
{
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 TexCoord;
};
struct D3D11_CONSTANT_BUFFER
{
	DirectX::XMMATRIX World;
};
class DirectXException
{
public:
    DirectXException(HRESULT hr);
    std::string HrToString(HRESULT hr)const;
};
#define ThrowIfFailed(hr) if (FAILED(hr)) { throw DirectXException(hr); }
