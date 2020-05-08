#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include "D3DCommon.h"
#define cos_radians(x) cos(static_cast<float>((x) * DirectX::XM_PI / 180.0f))
#define sin_radians(y) sin(static_cast<float>((y) * DirectX::XM_PI / 180.0f))
typedef class Window
{
private:
	HWND m_hwnd;
	WNDCLASS m_windowClass;
	LPCSTR m_windowClassName;
	DirectX::XMFLOAT2 windowSize;
public:
	DirectX::XMFLOAT3 m_OuterCamera;
	
	// x axis
	FLOAT m_Yaw = -90.0f;	
	// y axis
	FLOAT m_Pitch = 0.0f; 
	
	FLOAT m_Radius = 3.0f;

	FLOAT m_SpinSensitivity = 0.4f;
	FLOAT m_ZoomSensitivity = 0.1f;

	DirectX::XMFLOAT2 m_LastMouse;
	DirectX::XMFLOAT2 m_CurrMouse;

	BOOL m_bQuitMessagePosted;

	void CreateWin32Window(const LPCSTR windowTitleName, const INT x, const INT y, const INT w, const INT h, const HINSTANCE hInstance, const INT mCmdShow);
	BOOL RegisterWinClass();
	HWND GetWindow()const;
	DirectX::XMFLOAT2 getWindowSize() const;

	void MessageLoop(MSG msg);

	Window(Window* other) = delete;
	Window(const LPCSTR className, const HINSTANCE hInstance);
	~Window();
	
} Window, HWND32;

