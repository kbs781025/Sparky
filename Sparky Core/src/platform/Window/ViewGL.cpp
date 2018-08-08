#include "ViewGL.h"

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

	hglrc = ::wglCreateContext(hdc);

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
	PIXELFORMATDESCRIPTOR pfd;

	int pixelFormat = FindPixelFormat(hdc, colorBits, depthBits, stencilBits);
	if (pixelFormat == 0)
		return false;

	::DescribePixelFormat(hdc, pixelFormat, sizeof(pfd), &pfd);

	if (!::SetPixelFormat(hdc, pixelFormat, &pfd))
		return false;

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
