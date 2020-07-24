#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "EntityComponentSystem.h"
typedef class Transform : public Component
{
private:
	DirectX::XMFLOAT3 Translate;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT3 RotationAxis;
	FLOAT Angle;

	DirectX::XMMATRIX Model;
public:

	Transform();
	~Transform();

	Transform(Transform* other);
	Transform(DirectX::XMFLOAT3 translate, DirectX::XMFLOAT3 rotAxis, DirectX::XMFLOAT3 scale, FLOAT angle);
	
	void SetTranslation(DirectX::XMFLOAT3 translate);
	void SetRotationAxis(DirectX::XMFLOAT3 rotAxis);
	void SetScaling(DirectX::XMFLOAT3 scale);
	void SetAngle(FLOAT angle);
	void SetModelMatrix(DirectX::XMMATRIX matrix);
	void Zero();

	DirectX::XMFLOAT3 GetTranslation()const;
	DirectX::XMFLOAT3 GetRotationAxis()const;
	DirectX::XMFLOAT3 GetScaling()const;
	FLOAT GetAngle()const;
	DirectX::XMMATRIX GetModelMatrix() const;

} Transform, TransformComponent;