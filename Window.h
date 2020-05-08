#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include "D3DCommon.h"
#include "MathUtil.h"

class Camera;
class Window;
typedef class Window
{
private:
	HWND m_hwnd;
	WNDCLASS m_windowClass;
	LPCSTR m_windowClassName;
	DirectX::XMFLOAT2 windowSize;
	Camera* m_pCamera;
public:
	BOOL m_bQuitMessagePosted;

	void CreateWin32Window(const LPCSTR windowTitleName, const INT x, const INT y, const INT w, const INT h, const HINSTANCE hInstance, const INT mCmdShow);
	BOOL RegisterWinClass();
	HWND GetWindow()const;
	DirectX::XMFLOAT2 getWindowSize() const;

	Camera* GetCamera();

	void MessageLoop(MSG msg);

	Window(Window* other) = delete;
	Window(const LPCSTR className, const HINSTANCE hInstance, Camera& camera);
	~Window();

} Window, HWND32;

