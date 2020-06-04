#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <wrl.h>
#include "D3DUtil.h"
#include "EntityComponentSystem.h"

template <class T>
class IndexBufferComponent : public Component
{
public:
	IndexBufferComponent(IndexBufferComponent<T>* other);
	IndexBufferComponent();

	void Initialize(ID3D11Device* device, T* data, UINT numVerticies);
	void ZeroMem();

	UINT GetStride() const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer() const;
	
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_uStride;
};

template<class T>
inline IndexBufferComponent<T>::IndexBufferComponent(IndexBufferComponent<T>* other)
{
	this->m_buffer = other->GetBuffer();
	this->m_uStride = other->GetStride();
}

template<class T>
inline IndexBufferComponent<T>::IndexBufferComponent()
{
}

template<class T>
inline void IndexBufferComponent<T>::Initialize(ID3D11Device * device, T * data, UINT numIndicies)
{
	D3D11_BUFFER_DESC g_BufferDesc;
	ZeroMemory(&g_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	g_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
	g_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	g_BufferDesc.CPUAccessFlags = 0;
	g_BufferDesc.ByteWidth = sizeof(T) * numIndicies;

	D3D11_SUBRESOURCE_DATA g_SubResource;
	ZeroMemory(&g_SubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	g_SubResource.pSysMem = data;

	ThrowIfFailed(device->CreateBuffer(&g_BufferDesc, &g_SubResource, m_buffer.GetAddressOf()));
}

template<class T>
inline void IndexBufferComponent<T>::ZeroMem()
{
	ZeroMemory(m_buffer.GetAddressOf(), sizeof(ID3D11Buffer));
}

template<class T>
inline UINT IndexBufferComponent<T>::GetStride() const
{
	return sizeof(T);
}

template<class T>
inline Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBufferComponent<T>::GetBuffer() const
{
	return m_buffer;
}