#pragma once
#include <iostream>
#include <Windows.h>
#include <DirectXMath.h>

typedef class Window
{
private:
	HWND m_hwnd;
	WNDCLASS m_windowClass;
	LPCSTR m_windowClassName;
	DirectX::XMFLOAT2 windowSize;
public:
	BOOL quitMessagePosted;

	void createWindow(const LPCSTR windowTitleName, const INT x, const INT y, const INT w, const INT h, const HINSTANCE hInstance, const INT mCmdShow);
	BOOL registerClass();
	HWND getWindow()const;
	DirectX::XMFLOAT2 getWindowSize() const;

	void messageLoop(MSG msg);

	Window(Window* other) = delete;
	Window(const LPCSTR className, const HINSTANCE hInstance);
	~Window();
	
} Window, HWND32;

