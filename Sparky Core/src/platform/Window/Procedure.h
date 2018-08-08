#pragma once

#include <Windows.h>

namespace sparky { namespace win {

	LRESULT CALLBACK WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

	INT_PTR CALLBACK DialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

} }