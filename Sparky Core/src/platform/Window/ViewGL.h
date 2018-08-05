#pragma once

#include <Windows.h>

namespace sparky { namespace win {

	class ViewGL
	{
	public:
		ViewGL();
		~ViewGL();

		bool CreateContext(HWND handle, int colorBits, int depthBits, int stencilBits);
		void CloseContext(HWND handle);
		void SwapBuffer();

		HDC GetDC() const { return hdc; }
		HGLRC GetRC() const { return hglrc; }

	private:
		static bool SetPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
		static int FindPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);

		HDC hdc;
		HGLRC hglrc;
	};

}}