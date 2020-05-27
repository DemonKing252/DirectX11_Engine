#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#define MaxLights 1

struct PointLight
{
	DirectX::XMFLOAT3 Position;
	FLOAT SpecularStrength;

	DirectX::XMFLOAT3 Strength;
	FLOAT FallOffStart;

	DirectX::XMFLOAT3 Direction;
	FLOAT FallOffEnd;

};