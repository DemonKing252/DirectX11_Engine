#pragma once
#include <DirectXMath.h>
#include <Windows.h>
#include "D3DUtil.h"
#include "MathUtil.h"
class Camera
{
public:
	DirectX::XMFLOAT3 m_vEyePosition;

	FLOAT m_Yaw =  -90.0f;	// Y-Axis Rotation (Up)
	FLOAT m_Pitch =  0.0f;	// Z-Axis Rotation (Side)
	FLOAT m_Radius = 3.0f;	// Distance from vocal point

	FLOAT m_SpinSensitivity = 0.4f;		// yaw/pitch spin sensitivity
	FLOAT m_ZoomSensitivity = 0.1f;		// zoom sensitivity

	DirectX::XMFLOAT2 m_LastMouse;
	DirectX::XMFLOAT2 m_CurrMouse;

	Camera();
	~Camera();

	// methods

	void UpdateEyePosition();
	void Reset();
};