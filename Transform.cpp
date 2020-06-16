#include "Transform.h"

Transform::Transform()
{
}

Transform::~Transform()
{
}

Transform::Transform(Transform * other)
{
	this->Translate = other->GetTranslation();
	this->RotationAxis = other->GetRotationAxis();
	this->Scale = other->GetScaling();
	this->Angle = other->GetAngle();
}

Transform::Transform(DirectX::XMFLOAT3 translate, DirectX::XMFLOAT3 rotAxis, DirectX::XMFLOAT3 scale, FLOAT angle)
{
	this->Translate = translate;
	this->RotationAxis = rotAxis;
	this->Scale = scale;
	this->Angle = angle;
}

void Transform::SetTranslation(DirectX::XMFLOAT3 translate)
{
	this->Translate = translate;
}

void Transform::SetRotationAxis(DirectX::XMFLOAT3 rotAxis)
{
	this->RotationAxis = rotAxis;
}

void Transform::SetScaling(DirectX::XMFLOAT3 scale)
{
	this->Scale = scale;
}

void Transform::SetAngle(FLOAT angle)
{
	this->Angle = angle;
}

void Transform::SetModelMatrix(DirectX::XMMATRIX matrix)
{
	this->Model = matrix;
}

void Transform::Zero()
{
	this->Model = DirectX::XMMatrixIdentity();
	this->Scale = this->RotationAxis = this->Translate = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	this->Angle = 0.0f;

}

DirectX::XMMATRIX Transform::GetModelMatrix() const
{
	return DirectX::XMMATRIX();
}

DirectX::XMFLOAT3  Transform::GetTranslation()const
{
	return this->Translate;
}

DirectX::XMFLOAT3  Transform::GetRotationAxis()const
{
	return this->RotationAxis;
}

DirectX::XMFLOAT3  Transform::GetScaling()const
{
	return this->Scale;
}

FLOAT Transform::GetAngle()const
{
	return this->Angle;
}
