#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#define PointLightCount 2

struct PointLight
{
	DirectX::XMFLOAT3 Position;
	float FallOffStart;

	// 16 bytes
	DirectX::XMFLOAT3 Strength;
	float SpecularStrength;

	// 16 bytes
	DirectX::XMFLOAT3 Direction;
	float FallOffEnd;

};