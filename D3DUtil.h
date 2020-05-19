#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <string>
#include <wrl.h>

enum BufferType { VERTEX_BUFFER, INDEX_BUFFER, CONSTANT_BUFFER };
enum Shader { VS, PS };

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

	std::string HrToString(HRESULT hr)const;
	void Upload(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, BufferType bufferType, UINT byteSize, void* data);
private:
	static D3DUtil instance;
};
#define ThrowIfFailed(hr) if (FAILED(hr)) { throw std::runtime_error(D3DUtil::Get().HrToString(hr)); }