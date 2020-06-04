#pragma once
#include <d3d11.h>
#include <Windows.h>
#include <wrl.h>
#include "D3DUtil.h"
#include "EntityComponentSystem.h"
template <class T>
class VertexBufferComponent : public Component
{
public:
	VertexBufferComponent(VertexBufferComponent<T>* other);
	VertexBufferComponent();

	void Initialize(ID3D11Device* device, T* data, UINT numVerticies);
	void ZeroMem();

	UINT GetOffSet() const;
	UINT GetStride() const;
	Microsoft::WRL::ComPtr<ID3D11Buffer> GetBuffer() const;

private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_buffer;
	UINT m_uStride;
	UINT m_uOffSet;
};

template<class T>
inline VertexBufferComponent<T>::VertexBufferComponent(VertexBufferComponent<T>* other)
{
	this->m_buffer = other->GetBuffer();
	this->m_uStride = other->GetStride();
}

template<class T>
inline VertexBufferComponent<T>::VertexBufferComponent()
{
	m_uStride = sizeof(T);
	m_uOffSet = 0;
}

template<class T>
inline void VertexBufferComponent<T>::Initialize(ID3D11Device * device, T * data, UINT numVertices)
{
	D3D11_BUFFER_DESC g_BufferDesc;
	ZeroMemory(&g_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	
	g_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	g_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	g_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_BufferDesc.ByteWidth = sizeof(T) * numVertices;

	D3D11_SUBRESOURCE_DATA g_SubResource;
	ZeroMemory(&g_SubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	g_SubResource.pSysMem = data;
	
	ThrowIfFailed(device->CreateBuffer(&g_BufferDesc, &g_SubResource, m_buffer.GetAddressOf()));
}

template<class T>
inline void VertexBufferComponent<T>::ZeroMem()
{
	ZeroMemory(m_buffer.GetAddressOf(), sizeof(ID3D11Buffer));
}

template<class T>
inline UINT VertexBufferComponent<T>::GetOffSet() const
{
	return m_uOffSet;
}

template<class T>
inline UINT VertexBufferComponent<T>::GetStride() const
{
	return m_uStride;
}

template<class T>
inline Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBufferComponent<T>::GetBuffer() const
{
	return m_buffer;
}