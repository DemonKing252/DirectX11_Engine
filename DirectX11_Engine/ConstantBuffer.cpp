#include "ConstantBuffer.h"
void D3DUploadBuffer::Upload(ID3D11Device* device, ID3D11DeviceContext* deviceContext, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, Shader shader, UINT byteSize, void* data)
{
	D3D11_BUFFER_DESC cbvDesc;
	ZeroMemory(&cbvDesc, sizeof(D3D11_BUFFER_DESC));

	cbvDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbvDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbvDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbvDesc.ByteWidth = byteSize;

	D3D11_SUBRESOURCE_DATA g_SubResource;
	ZeroMemory(&g_SubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	g_SubResource.pSysMem = data;

	ThrowIfFailed(device->CreateBuffer(&cbvDesc, &g_SubResource, buffer.GetAddressOf()));

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