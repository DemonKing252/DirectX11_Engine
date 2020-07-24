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

PixelShaderController::PixelShaderController()
{
}

PixelShaderController::~PixelShaderController()
{
}

void PixelShaderController::ZeroMem()
{
	ZeroMemory(m_shader.GetAddressOf(), sizeof(ID3D11PixelShader));
}
