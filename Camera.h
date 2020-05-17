#pragma once
#include <DirectXMath.h>
#include <Windows.h>
class Camera
{
public:
	DirectX::XMFLOAT3 m_vEyePosition;

	// x axis
	FLOAT m_Yaw = -90.0f;
	// y axis
	FLOAT m_Pitch = 0.0f;

	FLOAT m_Radius = 3.0f;

	FLOAT m_SpinSensitivity = 0.4f;
	FLOAT m_ZoomSensitivity = 0.1f;

	DirectX::XMFLOAT2 m_LastMouse;
	DirectX::XMFLOAT2 m_CurrMouse;

	Camera()
	{

	}
	~Camera()	
	{
	}
};