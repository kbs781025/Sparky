#include "ViewGL.h"
#include <GL/glew.h>
#include <GL/wglew.h>
#include <iostream>

sparky::win::ViewGL::ViewGL()
	:
	hdc(0),
	hglrc(0)
{
}

sparky::win::ViewGL::~ViewGL()
{
}

bool sparky::win::ViewGL::CreateContext(HWND handle, int colorBits, int depthBits, int stencilBits)
{
	hdc = ::GetDC(handle);

	if (!SetPixelFormat(hdc, colorBits, depthBits, stencilBits))
	{
		::MessageBox(0, (LPCWSTR)L"Cannot set a suitable pixel format.", (LPCWSTR)L"Error", MB_ICONEXCLAMATION | MB_OK);
		::ReleaseDC(handle, hdc);
		return false;
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		std::cout << "Failed to initialize glew." << std::endl;
		return false;
	}

	::ReleaseDC(handle, hdc);

	return true;
}

void sparky::win::ViewGL::CloseContext(HWND handle)
{
	if (!hdc || !hglrc)
		return;

	::wglMakeCurrent(0, 0);
	::wglDeleteContext(hglrc);
	::ReleaseDC(handle, hdc);

	hdc = 0;
	hglrc = 0;
}

void sparky::win::ViewGL::SwapBuffer()
{
	::SwapBuffers(hdc);
}

bool sparky::win::ViewGL::SetPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
	int pixel_format_attribs[] = {
		WGL_DRAW_TO_WINDOW_ARB,     GL_TRUE,
		WGL_SUPPORT_OPENGL_ARB,     GL_TRUE,
		WGL_DOUBLE_BUFFER_ARB,      GL_TRUE,
		WGL_ACCELERATION_ARB,       WGL_FULL_ACCELERATION_ARB,
		WGL_PIXEL_TYPE_ARB,         WGL_TYPE_RGBA_ARB,
		WGL_COLOR_BITS_ARB,         colorBits,
		WGL_DEPTH_BITS_ARB,         depthBits,
		WGL_STENCIL_BITS_ARB,       stencilBits,
		0
	};

	int pixel_format;
	UINT num_formats;
	wglChoosePixelFormatARB(hdc, pixel_format_attribs, 0, 1, &pixel_format, &num_formats);
	if (!num_formats)
	{
		return false;
	}

	PIXELFORMATDESCRIPTOR pfd;

	::DescribePixelFormat(hdc, pixel_format, sizeof(pfd), &pfd);

	if (!::SetPixelFormat(hdc, pixel_format, &pfd))
		return false;

	int gl33_attribs[] = {
		WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
		WGL_CONTEXT_MINOR_VERSION_ARB, 3,
		WGL_CONTEXT_PROFILE_MASK_ARB,  WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0,
	};

	hglrc = wglCreateContextAttribsARB(hdc, 0, gl33_attribs);
	if (!hglrc)
	{
		return false;
	}

	if (!::wglMakeCurrent(hdc, hglrc))
	{
		return false;
	}

	return true;
}

int sparky::win::ViewGL::FindPixelFormat(HDC hdc, int colorBits, int depthBits, int stencilBits)
{
	int bestMode = 0;
	int bestScore = 0;
	PIXELFORMATDESCRIPTOR pfd;

	for (int currMode = 1; ::DescribePixelFormat(hdc, currMode, sizeof(pfd), &pfd); ++currMode)
	{
		if (!(pfd.dwFlags & PFD_SUPPORT_OPENGL))
			continue;
		if (!(pfd.dwFlags & PFD_DRAW_TO_WINDOW))
			continue;
		if ((pfd.iPixelType != PFD_TYPE_RGBA) || (pfd.dwFlags & PFD_NEED_PALETTE))
			continue;
		if (!(pfd.dwFlags & PFD_DOUBLEBUFFER))
			continue;

		int currScore = 0;

		if (pfd.cColorBits >= colorBits) ++currScore;
		if (pfd.cColorBits == colorBits) ++currScore;

		if (pfd.cDepthBits >= depthBits) ++currScore;
		if (pfd.cDepthBits == depthBits) ++currScore;

		if (pfd.cStencilBits >= stencilBits) ++currScore;
		if (pfd.cStencilBits == stencilBits) ++currScore;

		if (pfd.cAlphaBits > 0) ++currScore;

		if (currScore > bestScore)
		{
			bestScore = currScore;
			bestMode = currMode;
		}
	}

	return bestMode;
}

void sparky::win::ViewGL::CreateDummyContext(HWND DummyWin)
{
	HDC DummyDc = ::GetDC(DummyWin);

	PIXELFORMATDESCRIPTOR pfd = {};
	pfd.nSize = sizeof(pfd);
	pfd.nVersion = 1;
	pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
	pfd.iPixelType = PFD_TYPE_RGBA;
	pfd.cColorBits = 32;
	pfd.cAlphaBits = 8;
	pfd.iLayerType = PFD_MAIN_PLANE;
	pfd.cDepthBits = 24;
	pfd.cStencilBits = 8;

	int PixelFormat = ::ChoosePixelFormat(DummyDc, &pfd);
	if (!PixelFormat) return;
	::DescribePixelFormat(DummyDc, PixelFormat, sizeof(pfd), &pfd);

	if (!::SetPixelFormat(DummyDc, PixelFormat, &pfd))
	{	
		std::cout << ::GetLastError() << std::endl;
		return;
	}

	HGLRC DummyContext = ::wglCreateContext(DummyDc);
	if (!DummyContext) return;

	if (!::wglMakeCurrent(DummyDc, DummyContext)) return;

	wglCreateContextAttribsARB = (wglCreateContextAttribsARB_type*)::wglGetProcAddress("wglCreateContextAttribsARB");
	wglChoosePixelFormatARB = (wglChoosePixelFormatARB_type*)::wglGetProcAddress("wglChoosePixelFormatARB");

	::wglMakeCurrent(DummyDc, 0);
	::wglDeleteContext(DummyContext);
	::ReleaseDC(DummyWin, DummyDc);
	::DestroyWindow(DummyWin);

	return;
}
