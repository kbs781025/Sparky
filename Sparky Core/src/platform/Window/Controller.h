#pragma once

#include <Windows.h>

namespace sparky { namespace win {

	class Controller
	{
	public:
		Controller();
		virtual ~Controller();

		void SetHandle(HWND handle);

		virtual int Close();                                    // for WM_CLOSE
		virtual int Command(int id, int cmd, LPARAM msg);       // for WM_COMMAND: id, cmd, msg
		virtual int ContextMenu(HWND handle, int x, int y);     // for WM_CONTEXTMENU: handle, x, y
		virtual int Create();                                   // for WM_CREATE
		virtual int Destroy();                                  // for WM_DESTROY
		virtual int Enable(bool flag);                          // for WM_ENABLE: flag(true/false)
		virtual int EraseBkgnd(HDC hdc);                        // for WM_ERASEBKGND: HDC
		virtual int HScroll(WPARAM wParam, LPARAM lParam);      // for WM_HSCROLL
		virtual int KeyDown(int key, LPARAM lParam);            // for WM_KEYDOWN: keyCode, detailInfo
		virtual int KeyUp(int key, LPARAM lParam);              // for WM_KEYUP: keyCode, detailInfo
		virtual int LButtonDown(WPARAM state, int x, int y);    // for WM_LBUTTONDOWN: state, x, y
		virtual int LButtonUp(WPARAM state, int x, int y);      // for WM_LBUTTONUP: state, x, y
		virtual int MButtonDown(WPARAM state, int x, int y);    // for WM_MBUTTONDOWN: state, x, y
		virtual int MButtonUp(WPARAM state, int x, int y);      // for WM_MBUTTONUP: state, x, y
		virtual int MouseHover(int state, int x, int y);        // for WM_MOUSEHOVER: state, x, y
		virtual int MouseLeave();                               // for WM_MOUSELEAVE
		virtual int MouseMove(WPARAM state, int x, int y);      // for WM_MOUSEMOVE: state, x, y
		virtual int MouseWheel(int state, int d, int x, int y); // for WM_MOUSEWHEEL: state, delta, x, y
		virtual int Notify(int id, LPARAM lParam);              // for WM_NOTIFY: controllerID, NMHDR
		virtual int Paint();                                    // for WM_PAINT
		virtual int RButtonDown(WPARAM wParam, int x, int y);   // for WM_RBUTTONDOWN: state, x, y
		virtual int RButtonUp(WPARAM wParam, int x, int y);     // for WM_RBUTTONUP: state, x, y
		virtual int Size(int w, int h, WPARAM wParam);          // for WM_SIZE: width, height, type(SIZE_MAXIMIZED...)
		virtual int Timer(WPARAM id, LPARAM lParam);            // for WM_TIMER: ID, ptr to callback ftn
		virtual int VScroll(WPARAM wParam, LPARAM lParam);      // for WM_VSCROLL
	
	protected:
		HWND handle;
	
	private:

	};

	Controller::Controller()
		:
		handle(0)
	{
	}

	Controller::~Controller()
	{
		::DestroyWindow(handle);
	}

	inline void Controller::SetHandle(HWND hwnd) { handle = hwnd; }
	inline int Controller::Close() { ::DestroyWindow(handle); return 0; }
	inline int Controller::Command(int id, int cmd, LPARAM msg) { return 0; }
	inline int Controller::ContextMenu(HWND handle, int x, int y) { return 0; }
	inline int Controller::Create() { return 0; }
	inline int Controller::Destroy() { return 0; }
	inline int Controller::Enable(bool flag) { return 0; }
	inline int Controller::EraseBkgnd(HDC hdc) { return 0; }
	inline int Controller::HScroll(WPARAM wParam, LPARAM lParam) { return 0; }
	inline int Controller::KeyDown(int key, LPARAM lParam) { return 0; }
	inline int Controller::KeyUp(int key, LPARAM lParam) { return 0; }
	inline int Controller::LButtonDown(WPARAM wParam, int x, int y) { return 0; }
	inline int Controller::LButtonUp(WPARAM wParam, int x, int y) { return 0; }
	inline int Controller::MButtonDown(WPARAM wParam, int x, int y) { return 0; }
	inline int Controller::MButtonUp(WPARAM wParam, int x, int y) { return 0; }
	inline int Controller::MouseHover(int state, int x, int y) { return 0; }
	inline int Controller::MouseLeave() { return 0; }
	inline int Controller::MouseMove(WPARAM keyState, int x, int y) { return 0; }
	inline int Controller::MouseWheel(int state, int delta, int x, int y) { return 0; }
	inline int Controller::Notify(int id, LPARAM lParam) { return 0; }
	inline int Controller::Paint() { return 0; }
	inline int Controller::RButtonDown(WPARAM wParam, int x, int y) { return 0; }
	inline int Controller::RButtonUp(WPARAM wParam, int x, int y) { return 0; }
	inline int Controller::Size(int w, int h, WPARAM type) { return 0; }
	inline int Controller::Timer(WPARAM id, LPARAM lParam) { return 0; }
	inline int Controller::VScroll(WPARAM wParam, LPARAM lParam) { return 0; }

} }