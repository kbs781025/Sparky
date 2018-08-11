#pragma once

#include <Windows.h>

namespace sparky { namespace win {

	class ViewGL
	{
	public:
		ViewGL();
		~ViewGL();

		bool CreateContext(HWND handle, int colorBits, int depthBits, int stencilBits);
		void CreateDummyContext(HWND DummyWin);
		void CloseContext(HWND handle);
		void SwapBuffer();

		HDC GetDC() const { return hdc; }
		HGLRC GetRC() const { return hglrc; }

	private:
		typedef HGLRC WINAPI wglCreateContextAttribsARB_type(HDC hdc, HGLRC hShareContext, const int* attribList);
		typedef BOOL WINAPI wglChoosePixelFormatARB_type(HDC hdc, const int* piAttribList, const FLOAT* pfAttribFlist, UINT nMaxFormats, int* piFormats, UINT* nNumFormats);
		wglCreateContextAttribsARB_type *wglCreateContextAttribsARB;
		wglChoosePixelFormatARB_type* wglChoosePixelFormatARB;

	private:
		bool SetPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);
		int FindPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits);

		HDC hdc;
		HGLRC hglrc;
	};

}}