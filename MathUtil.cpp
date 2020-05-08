#include "MathUtil.h"
DirectX::XMMATRIX MathUtil::InverseTranspose(DirectX::CXMMATRIX M)
{
	// B = (A−1)T (the inverse transpose of A) 
	DirectX::XMMATRIX A = M;
	A.r[3] = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);
	DirectX::XMVECTOR det = DirectX::XMMatrixDeterminant(A);
	return DirectX::XMMatrixTranspose(DirectX::XMMatrixInverse(&det, A));
	
}

double MathUtil::cos_radians(double _x)
{
	return cos(static_cast<float>((_x)* DirectX::XM_PI / 180.0f));
}

double MathUtil::sin_radians(double _y)
{
	return sin(static_cast<float>((_y)* DirectX::XM_PI / 180.0f));
}
