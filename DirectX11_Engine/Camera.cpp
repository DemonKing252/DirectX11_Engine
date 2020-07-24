#include "Camera.h"

Camera::Camera()
{
}

Camera::~Camera()
{
}

void Camera::UpdateEyePosition()
{
	this->m_vEyePosition.x = MathUtil::cos_radians(this->m_Yaw) * MathUtil::cos_radians(this->m_Pitch);
	this->m_vEyePosition.y = MathUtil::sin_radians(this->m_Pitch);						
	this->m_vEyePosition.z = MathUtil::sin_radians(this->m_Yaw) * MathUtil::cos_radians(this->m_Pitch);

	D3DUtil::Get().m_EyePos = DirectX::XMVectorSet
	(
		this->m_Radius * this->m_vEyePosition.x,
		this->m_Radius * this->m_vEyePosition.y,
		this->m_Radius * this->m_vEyePosition.z,
		1.0f
	);
}

void Camera::Reset()
{
	m_Yaw = -90.0f;
	m_Pitch = 0.0f;
	m_Radius = 3.0f;

	this->UpdateEyePosition();
}