#pragma once
#include <DirectXMath.h>
#include <Windows.h>
class MathUtil
{
public:
	static DirectX::XMMATRIX InverseTranspose(DirectX::CXMMATRIX M);

	static double cos_radians(double _x);
	static double sin_radians(double _y);

};

