#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include <d3d11.h>
struct PointLight
{
	DirectX::XMFLOAT3 Position;
	FLOAT SpecularStrength;

	DirectX::XMFLOAT3 Strength;
	FLOAT FallOffStart;

	DirectX::XMFLOAT3 Direction;	
	FLOAT FallOffEnd;

};
