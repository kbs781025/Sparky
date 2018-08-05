#pragma once

#include <Windows.h>

namespace sparky { namespace win {

	LRESULT CALLBACK _WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	INT_PTR CALLBACK _DialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

} }