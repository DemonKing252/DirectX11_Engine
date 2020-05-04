#include "Window.h"
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(EXIT_SUCCESS);
		break;
	case WM_SIZE:
		// Size call back
		break;

	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


Window::Window(const LPCSTR className, const HINSTANCE hInstance)
{
	quitMessagePosted = false;

	// Step1: create a window class
	ZeroMemory(&m_windowClass, sizeof(WNDCLASS));

	this->m_windowClassName = className;
	m_windowClass.style = CS_OWNDC;
	m_windowClass.lpfnWndProc = WindowProc;
	m_windowClass.cbClsExtra = 0;		// Zero extra bytes in the class structure
	m_windowClass.cbWndExtra = 0;		// Zero extra bytes for every window created 
	m_windowClass.hInstance = hInstance;
	m_windowClass.hIcon = nullptr;
	m_windowClass.hCursor = nullptr;
	m_windowClass.hbrBackground = nullptr;
	m_windowClass.lpszMenuName = nullptr;			// No menu
	m_windowClass.lpszClassName = className;

	// Step 2: create a window using the associated class
}


Window::~Window()
{
}
void Window::createWindow(const LPCSTR windowTitleName, const INT x, const INT y, const INT w, const INT h, const HINSTANCE hInstance, const INT mCmdShow)
{
	windowSize = DirectX::XMFLOAT2(w, h);
	m_hwnd = CreateWindowEx(
		NULL,
		m_windowClassName,
		windowTitleName,
		WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU | WS_MAXIMIZEBOX | WS_SIZEBOX,
		x, y, w, h,
		NULL, NULL, hInstance, NULL
	);

	ShowWindow(m_hwnd, mCmdShow);
}	

BOOL Window::registerClass()
{
	if (!SUCCEEDED(RegisterClass(&m_windowClass)))
		return FALSE;
	
	return TRUE;
}

HWND Window::getWindow() const
{
	return m_hwnd;
}

DirectX::XMFLOAT2 Window::getWindowSize() const
{
	return windowSize;
}

void Window::messageLoop(MSG msg)
{	
	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT)
			quitMessagePosted = true;

	}
	
}
