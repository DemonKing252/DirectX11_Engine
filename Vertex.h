#pragma once
#include <DirectXMath.h>
class Vertex
{
public:
	void SetPosition(DirectX::XMFLOAT3 position);
	void SetUV(DirectX::XMFLOAT2 uv);
	void SetNormal(DirectX::XMFLOAT3 normal);

	DirectX::XMFLOAT3 GetPosition()const;
	DirectX::XMFLOAT2 GetUV()const;
	DirectX::XMFLOAT3 GetNormal()const;

	Vertex();
	Vertex(Vertex* other);
	Vertex(DirectX::XMFLOAT3 position, DirectX::XMFLOAT2 uv, DirectX::XMFLOAT3 normal);
	
	void Zero();
	void operator=(Vertex other);
	bool operator==(Vertex other);


private:
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT2 UV;
	DirectX::XMFLOAT3 Normal;
};