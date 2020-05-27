#include "D3DUtil.h"
D3DUtil D3DUtil::instance = D3DUtil();
std::string D3DUtil::HrToString(HRESULT hr) const
{
	char s_str[64] = {};
	sprintf_s(s_str, "HRESULT of 0x%08X", static_cast<UINT>(hr));
	return std::string(s_str);
}
void D3DUtil::Upload(ID3D11Device* device, Microsoft::WRL::ComPtr<ID3D11Buffer>& buffer, BufferType bufferType, UINT byteSize, void* data)
{
	D3D11_BUFFER_DESC g_BufferDesc;
	ZeroMemory(&g_BufferDesc, sizeof(D3D11_BUFFER_DESC));
	switch (bufferType)
	{
	case VERTEX_BUFFER:
		g_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		g_BufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
		g_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	case INDEX_BUFFER:
		g_BufferDesc.Usage = D3D11_USAGE_DEFAULT;
		g_BufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
		g_BufferDesc.CPUAccessFlags = 0;
		break;
	case CONSTANT_BUFFER:
		g_BufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		g_BufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		g_BufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		break;
	}
	g_BufferDesc.ByteWidth = byteSize;

	D3D11_SUBRESOURCE_DATA g_SubResource;
	ZeroMemory(&g_SubResource, sizeof(D3D11_SUBRESOURCE_DATA));
	g_SubResource.pSysMem = data;

	ThrowIfFailed(device->CreateBuffer(&g_BufferDesc, &g_SubResource, buffer.GetAddressOf()));
}