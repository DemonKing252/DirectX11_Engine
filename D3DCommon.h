#pragma once
#include <DirectXMath.h>
#include "EntityComponentSystem.h"
#include <Windows.h>
#include <d3d11.h>
#include <wrl.h>
#define PointLightCount 2

enum Shader { VS, PS };
enum PixelShader { Default, NoIllumination, Undefined };

class PointLight
{
public:
	DirectX::XMFLOAT3 Position;
	float FallOffStart;

	// 16 bytes
	DirectX::XMFLOAT3 Strength;
	float SpecularStrength;

	// 16 bytes
	DirectX::XMFLOAT3 Direction;
	float FallOffEnd;

};

class ShaderResourceViewComponent : public Component
{
public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_shaderResource;
	
	ShaderResourceViewComponent();
	~ShaderResourceViewComponent();

	void ZeroMem();

};

class PixelShaderController : public Component
{
public:
	PixelShader m_shaderType;
	Microsoft::WRL::ComPtr<ID3D11PixelShader> m_shader;

	PixelShaderController();
	~PixelShaderController();

	void ZeroMem();
};