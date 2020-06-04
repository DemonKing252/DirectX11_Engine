#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>
#include <string>
#include "D3DCommon.h"
#include "D3DUtil.h"
#include "MathUtil.h"
class Camera;
class Window;
typedef class Window
{
public:
	Window(Window* other) = delete;
	Window(const LPCSTR className, const HINSTANCE hInstance, std::shared_ptr<Camera> camera);
	~Window();

	void Create(const LPCSTR windowTitleName, const INT x, const INT y, const INT w, const INT h, const HINSTANCE hInstance, const INT mCmdShow);
	void MessageLoop(MSG msg);
	void Clean();

	BOOL Register();
	HWND GetWindow()const;
	
	std::shared_ptr<Camera> GetCamera();
	DirectX::XMFLOAT2 GetFrameBufferSize() const;

	BOOL m_bQuitMessagePosted;
private:
	HWND m_hwnd;
	WNDCLASS m_windowClass;
	LPCSTR m_windowClassName;
	HINSTANCE m_hInstance;

	DirectX::XMFLOAT2 windowSize;
	std::shared_ptr<Camera> m_pCamera;
} Window, HWND32;