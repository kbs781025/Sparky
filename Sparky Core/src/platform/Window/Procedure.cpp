#include "Procedure.h"
#include "Controller.h"

LRESULT sparky::win::_WindowProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT ReturnValue = 0;

	static win::Controller* pController;
	pController = (Controller*)::GetWindowLongPtr(hwnd, GWLP_USERDATA);

	if (msg == WM_NCCREATE)
	{
		pController = (Controller*)((CREATESTRUCT*)lParam)->lpCreateParams;
		pController->SetHandle(hwnd);
		::SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pController);

		return ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	if (!pController)
		return ::DefWindowProc(hwnd, msg, wParam, lParam);

	switch (msg)
	{
	case WM_CREATE:
		ReturnValue = pController->Create();
		break;

	case WM_SIZE:
		ReturnValue = pController->Size(LOWORD(lParam), HIWORD(lParam), (int)wParam);
		break;

	case WM_ENABLE:
	{
		bool flag;
		if (wParam) flag = true;
		else flag = false;
		ReturnValue = pController->Enable(flag);
		break;
	}

	case WM_PAINT:
		pController->Paint();
		ReturnValue = ::DefWindowProc(hwnd, msg, wParam, lParam);
		break;

	case WM_COMMAND:
		ReturnValue = pController->Command(LOWORD(wParam), HIWORD(wParam), lParam);
		break;

	case WM_CLOSE:
		ReturnValue = pController->Close();
		break;

	case WM_DESTROY:
		ReturnValue = pController->Destroy();
		break;

	case WM_SYSCOMMAND:
		ReturnValue = ::DefDlgProc(hwnd, msg, wParam, lParam);
		break;

	case WM_CHAR:
		//ReturnValue = pController->char(wParam, lParam);   // route keycode
		break;

	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		ReturnValue = pController->KeyDown((int)wParam, lParam);                       // keyCode, keyDetail
		break;

	case WM_KEYUP:
	case WM_SYSKEYUP:
		ReturnValue = pController->KeyUp((int)wParam, lParam);                         // keyCode, keyDetail
		break;

	case WM_LBUTTONDOWN:
		ReturnValue = pController->LButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); // state, x, y
																				 //ReturnValue = pController->lButtonDown(wParam, GET_X_LPARAM(lParam), GET_X_LPARAM(lParam)); // state, x, y
		break;

	case WM_LBUTTONUP:
		ReturnValue = pController->LButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));   // state, x, y
		break;

	case WM_RBUTTONDOWN:
		ReturnValue = pController->RButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); // state, x, y
		break;

	case WM_RBUTTONUP:
		ReturnValue = pController->RButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));   // state, x, y
		break;

	case WM_MBUTTONDOWN:
		ReturnValue = pController->MButtonDown(wParam, LOWORD(lParam), HIWORD(lParam)); // state, x, y
		break;

	case WM_MBUTTONUP:
		ReturnValue = pController->MButtonUp(wParam, LOWORD(lParam), HIWORD(lParam));   // state, x, y
		break;

	case WM_MOUSEHOVER:
		ReturnValue = pController->MouseHover((int)wParam, (short)LOWORD(lParam), (short)HIWORD(lParam));   // state, x, y
		break;

	case WM_MOUSELEAVE:
		ReturnValue = pController->MouseLeave();
		break;

	case WM_MOUSEMOVE:
		ReturnValue = pController->MouseMove(wParam, LOWORD(lParam), HIWORD(lParam));  // state, x, y
		break;

	case WM_MOUSEWHEEL:
		ReturnValue = pController->MouseWheel((short)LOWORD(wParam), (short)HIWORD(wParam) / WHEEL_DELTA, (short)LOWORD(lParam), (short)HIWORD(lParam));   // state, delta, x, y
		break;

	case WM_HSCROLL:
		ReturnValue = pController->HScroll(wParam, lParam);
		break;

	case WM_VSCROLL:
		ReturnValue = pController->VScroll(wParam, lParam);
		break;

	case WM_TIMER:
		ReturnValue = pController->Timer(LOWORD(wParam), HIWORD(wParam));
		break;

	case WM_NOTIFY:
		ReturnValue = pController->Notify((int)wParam, lParam);                        // controllerID, lParam
		break;

	case WM_CONTEXTMENU:
		ReturnValue = pController->ContextMenu((HWND)wParam, LOWORD(lParam), HIWORD(lParam));

	default:
		ReturnValue = ::DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return ReturnValue;
}

INT_PTR sparky::win::_DialogProcedure(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	return INT_PTR();
}
