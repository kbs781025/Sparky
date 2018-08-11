#pragma once

#include "Controller.h"
#include "ViewGL.h"
#include "../opengl/ModelGL.h"
#include <thread>

namespace sparky { namespace win {

	using namespace sparky;

	class ControllerGL : public Controller
	{
	public:
		ControllerGL(opengl::ModelGL* model, ViewGL* view);
		~ControllerGL() {};

		int Close();
		int Command(int id, int cmd, LPARAM msg);   // for WM_COMMAND
		int Create();                               // create RC for OpenGL window and start new thread for rendering
		int Destroy();
		int Paint();
		int LButtonDown(WPARAM state, int x, int y);
		int LButtonUp(WPARAM state, int x, int y);
		int RButtonDown(WPARAM state, int x, int y);
		int RButtonUp(WPARAM state, int x, int y);
		int MouseMove(WPARAM state, int x, int y);
		int KeyDown(int key, LPARAM lParam);
		int Size(int width, int height, WPARAM type);

		int CreateDummyContext(HWND hwnd);

	private:
		void RunThread();

		opengl::ModelGL* m_ModelGL;
		ViewGL* m_ViewGL;
		volatile bool m_LoopFlag;
		std::thread m_GLthread;
		bool m_ResizeFlag;
		int m_ClientWidth;
		int m_ClientHeight;
	};

} }