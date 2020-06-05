#include "D3DCommon.h"

ShaderResourceViewComponent::ShaderResourceViewComponent()
{
}

ShaderResourceViewComponent::~ShaderResourceViewComponent()
{
}

void ShaderResourceViewComponent::ZeroMem()
{
	ZeroMemory(m_shaderResource.GetAddressOf(), sizeof(ID3D11ShaderResourceView));
}
