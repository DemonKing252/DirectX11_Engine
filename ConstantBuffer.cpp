#include "ConstantBuffer.h"
void D3DUploadBuffer::Upload(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, Shader shader, UINT byteSize, void* data)
{
	D3D11_BUFFER_DESC g_BufferDesc;
	ZeroMemory(&g_BufferDesc, sizeof(D3D11_BUFFER_DESC));

	g_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	g_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	g_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	g_BufferDesc.ByteWidth = byteSize;

	D3D11_SUBRESOURCE_DATA g_SubResource;
	ZeroMemory(&g_SubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	g_SubResource.pSysMem = data;

	ThrowIfFailed(device->CreateBuffer(&g_BufferDesc, &g_SubResource, buffer.GetAddressOf()));

	switch (shader)
	{
	case VS:
		deviceContext->VSSetConstantBuffers(0, 1, buffer.GetAddressOf());
		break;
	case PS:
		deviceContext->PSSetConstantBuffers(0, 1, buffer.GetAddressOf());
		break;
	}
}