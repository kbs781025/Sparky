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
		HWND GetHandle();

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
