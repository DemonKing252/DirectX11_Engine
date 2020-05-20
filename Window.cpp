#include "Window.h"
#include "Camera.h"

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
	case WM_LBUTTONDOWN:
		//if (wParam & MK_LBUTTON)
		//Util::leftPressed = true;
		break;
	case WM_LBUTTONUP:
		//Util::leftPressed = false;
		break;
	case WM_MOUSEMOVE:
		break;
	}

	return DefWindowProc(hwnd, msg, wParam, lParam);
}


Window::Window(const LPCSTR className, const HINSTANCE hInstance, std::shared_ptr<Camera> camera)
{
	m_bQuitMessagePosted = false;

	this->m_hInstance = hInstance;
	this->m_pCamera = camera;
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

	m_pCamera->m_vEyePosition.x = MathUtil::cos_radians(m_pCamera->m_Yaw) * MathUtil::cos_radians(m_pCamera->m_Pitch);
	m_pCamera->m_vEyePosition.y = MathUtil::sin_radians(m_pCamera->m_Pitch);
	m_pCamera->m_vEyePosition.z = MathUtil::sin_radians(m_pCamera->m_Yaw) * MathUtil::cos_radians(m_pCamera->m_Pitch);

	D3DUtil::Get().m_EyePos = DirectX::XMVectorSet
	(
		m_pCamera->m_Radius * m_pCamera->m_vEyePosition.x,
		m_pCamera->m_Radius * m_pCamera->m_vEyePosition.y,
		m_pCamera->m_Radius * m_pCamera->m_vEyePosition.z,
		1.0f
	);
	
}


Window::~Window()
{
}
void Window::CreateWin32Window(const LPCSTR windowTitleName, const INT x, const INT y, const INT w, const INT h, const HINSTANCE hInstance, const INT mCmdShow)
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
	SetForegroundWindow(m_hwnd);
	SetFocus(m_hwnd);
}	

BOOL Window::RegisterWinClass()
{
	if (!SUCCEEDED(RegisterClass(&m_windowClass)))
		return FALSE;
	
	return TRUE;
}

HWND Window::GetWindow() const
{
	return m_hwnd;
}

DirectX::XMFLOAT2 Window::getWindowSize() const
{
	return windowSize;
}

std::shared_ptr<Camera> Window::GetCamera()
{
	return m_pCamera;
	// TODO: insert return statement here
}

void Window::MessageLoop(MSG msg)
{	
	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if (msg.message == WM_QUIT) 
			m_bQuitMessagePosted = true; 
			
		if (msg.message == WM_LBUTTONDOWN)
		{
			D3DUtil::Get().leftPressed = true;
			m_pCamera->m_CurrMouse = DirectX::XMFLOAT2(LOWORD(msg.lParam), HIWORD(msg.lParam));
		}

		if (msg.message == WM_LBUTTONUP) 
			D3DUtil::Get().leftPressed = false; 
		
		if (msg.message == WM_RBUTTONDOWN)
		{
			D3DUtil::Get().rightPressed = true;
			m_pCamera->m_CurrMouse = DirectX::XMFLOAT2(LOWORD(msg.lParam), HIWORD(msg.lParam));
		}
		

		if (msg.message == WM_RBUTTONUP) 
			D3DUtil::Get().rightPressed = false;	
		
			
		if (D3DUtil::Get().leftPressed)
		{
			m_pCamera->m_LastMouse = m_pCamera->m_CurrMouse;

			D3DUtil::Get().m_mousePos = DirectX::XMFLOAT2(LOWORD(msg.lParam), HIWORD(msg.lParam));
			m_pCamera->m_CurrMouse = D3DUtil::Get().m_mousePos;
			
			m_pCamera->m_Yaw -= static_cast<FLOAT>(m_pCamera->m_CurrMouse.x - m_pCamera->m_LastMouse.x) * m_pCamera->m_SpinSensitivity;
			
			if ((static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_SpinSensitivity) > 0.0f &&
				m_pCamera->m_Pitch >= -89.9f + (static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_SpinSensitivity))
				m_pCamera->m_Pitch -= static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_SpinSensitivity;

			else if ((static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_SpinSensitivity) < 0.0f &&
				m_pCamera->m_Pitch <= +87.5f - (static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_SpinSensitivity))
				m_pCamera->m_Pitch -= static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_SpinSensitivity;
			
			m_pCamera->m_vEyePosition.x = MathUtil::cos_radians(m_pCamera->m_Yaw) * MathUtil::cos_radians(m_pCamera->m_Pitch);
			m_pCamera->m_vEyePosition.y = MathUtil::sin_radians(m_pCamera->m_Pitch);
			m_pCamera->m_vEyePosition.z = MathUtil::sin_radians(m_pCamera->m_Yaw) * MathUtil::cos_radians(m_pCamera->m_Pitch);

			D3DUtil::Get().m_EyePos = DirectX::XMVectorSet
			(
				m_pCamera->m_Radius * m_pCamera->m_vEyePosition.x,
				m_pCamera->m_Radius * m_pCamera->m_vEyePosition.y,
				m_pCamera->m_Radius * m_pCamera->m_vEyePosition.z,
				1.0f
			);

		}
		if (D3DUtil::Get().rightPressed)
		{
			m_pCamera->m_LastMouse = m_pCamera->m_CurrMouse;

			D3DUtil::Get().m_mousePos = DirectX::XMFLOAT2(LOWORD(msg.lParam), HIWORD(msg.lParam));
			m_pCamera->m_CurrMouse = D3DUtil::Get().m_mousePos;

			if (static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_ZoomSensitivity < 0.0f &&
				m_pCamera->m_Radius >= 2.0f + static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_ZoomSensitivity)
				m_pCamera->m_Radius += static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_ZoomSensitivity;
			
			else if (static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_ZoomSensitivity > 0.0f &&
				m_pCamera->m_Radius <= 20.0f + static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_ZoomSensitivity)
				m_pCamera->m_Radius += static_cast<FLOAT>(m_pCamera->m_CurrMouse.y - m_pCamera->m_LastMouse.y) * m_pCamera->m_ZoomSensitivity;

			m_pCamera->m_vEyePosition.x = MathUtil::cos_radians(m_pCamera->m_Yaw) * MathUtil::cos_radians(m_pCamera->m_Pitch);
			m_pCamera->m_vEyePosition.y = MathUtil::sin_radians(m_pCamera->m_Pitch);
			m_pCamera->m_vEyePosition.z = MathUtil::sin_radians(m_pCamera->m_Yaw) * MathUtil::cos_radians(m_pCamera->m_Pitch);

			D3DUtil::Get().m_EyePos = DirectX::XMVectorSet(m_pCamera->m_Radius * m_pCamera->m_vEyePosition.x,
				m_pCamera->m_Radius * m_pCamera->m_vEyePosition.y,
				m_pCamera->m_Radius * m_pCamera->m_vEyePosition.z,
				1.0f);

		}
	}	
	
}

void Window::Clean()
{
	UnregisterClass(m_windowClassName, m_hInstance);
	DestroyWindow(m_hwnd);
}
