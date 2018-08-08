#include "Window.h"
#include "Procedure.h"
#include <cwchar>

sparky::win::Window::Window(HINSTANCE hInst, const wchar_t * name, HWND hParent, Controller * controller)
	:
	m_Handle(0), m_Instance(hInst), 
	m_Controller(controller),
	m_WinStyle(WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN), 
	m_WinStyleEx(WS_EX_CLIENTEDGE),
	x(CW_USEDEFAULT), y(CW_USEDEFAULT),
	m_Width(CW_USEDEFAULT), m_Height(CW_USEDEFAULT),
	m_ParentHandle(hParent), m_MenuHandle(0)
{
	// copy string
	wcsncpy(this->m_Title, name, MAX_STRING - 1);
	wcsncpy(this->m_ClassName, name, MAX_STRING - 1);

	m_WinClass.cbSize = sizeof(WNDCLASSEX);
	m_WinClass.style = 0;
	m_WinClass.lpfnWndProc = win::WindowProcedure; 
	m_WinClass.cbClsExtra = 0;
	m_WinClass.cbWndExtra = 0;
	m_WinClass.hInstance = m_Instance;
	m_WinClass.hIcon = LoadIcon(m_Instance, IDI_APPLICATION);
	m_WinClass.hIconSm = 0;
	m_WinClass.hCursor = LoadCursor(0, IDC_ARROW);
	m_WinClass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	m_WinClass.lpszMenuName = 0;
	m_WinClass.lpszClassName = m_ClassName;
	m_WinClass.hIconSm = LoadIcon(m_Instance, IDI_APPLICATION);
}

sparky::win::Window::~Window()
{
	::UnregisterClass(m_ClassName, m_Instance);
}

HWND sparky::win::Window::Create()
{
	if (!::RegisterClassEx(&m_WinClass)) return 0;

	m_Handle = ::CreateWindowEx(m_WinStyleEx,
								m_ClassName,
								m_Title,
								m_WinStyle,
								x, y,
								m_Width, m_Height,
								m_ParentHandle,
								m_MenuHandle,
								m_Instance,
								(LPVOID)m_Controller);

	return m_Handle;
}

void sparky::win::Window::Show(int cmdShow)
{
	::ShowWindow(m_Handle, cmdShow);
	::UpdateWindow(m_Handle);
}

HICON sparky::win::Window::_LoadIcon(int id)
{
	return (HICON)::LoadImage(m_Instance, MAKEINTRESOURCE(id), IMAGE_ICON, 0, 0, LR_DEFAULTSIZE);
}

HCURSOR sparky::win::Window::_LoadCursor(int id)
{
	return (HCURSOR)::LoadImage(m_Instance, MAKEINTRESOURCE(id), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE);
}
