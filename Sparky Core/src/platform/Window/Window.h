#pragma once

#include <windows.h>
#include "Controller.h"

namespace sparky { namespace win {

	class Window
	{
	public:
		Window(HINSTANCE hInst, const wchar_t* name, HWND hParent, Controller* controller);
		~Window();

		HWND Create();
		void Show(int cmdShow = SW_SHOWDEFAULT);
		HWND GetHandle() { return m_Handle; };

		void InitializeOpenGL();

		void SetClassStyle(UINT style) { m_WinClass.style = style; };
		void SetIcon(int id) { m_WinClass.hIcon = _LoadIcon(id); };
		void SetIconSmall(int id) { m_WinClass.hIconSm = _LoadIcon(id); };
		void SetCursor(int id) { m_WinClass.hCursor = _LoadCursor(id); };
		void SetBackground(int color) { m_WinClass.hbrBackground = (HBRUSH)::GetStockObject(color); };
		void SetMenuName(LPCTSTR name) { m_WinClass.lpszMenuName = name; };

		void SetWindowStyle(DWORD style) { m_WinStyle = style; };
		void SetWindowStyleEx(DWORD style) { m_WinStyleEx = style; };
		void SetPosition(int x, int y) { this->x = x; this->y = y; };
		void SetWidth(int w) { m_Width = w; };
		void SetHeight(int h) { m_Height = h; };
		void SetParent(HWND handle) { m_ParentHandle = handle; };
		void SetMenu(HMENU handle) { m_MenuHandle = handle; };

	private:
		enum { MAX_STRING = 256 };
		HICON _LoadIcon(int id);
		HCURSOR _LoadCursor(int id);

		HWND m_Handle;
		WNDCLASSEX m_WinClass;
		DWORD m_WinStyle;
		DWORD m_WinStyleEx;
		wchar_t m_Title[MAX_STRING];
		wchar_t m_ClassName[MAX_STRING];
		int x;
		int y;
		int m_Width;
		int m_Height;
		HWND m_ParentHandle;
		HMENU m_MenuHandle;
		HINSTANCE m_Instance;
		Controller* m_Controller;
	};

} }
