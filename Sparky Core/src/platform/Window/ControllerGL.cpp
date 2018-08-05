#include "ControllerGL.h"

sparky::win::ControllerGL::ControllerGL(opengl::ModelGL * model, ViewGL * view)
	:
	m_ModelGL(model),
	m_ViewGL(view),
	m_LoopFlag(false),
	m_ResizeFlag(false),
	m_ClientWidth(0), m_ClientHeight(0)
{
}

int sparky::win::ControllerGL::Close()
{
	m_LoopFlag = false;
	m_GLthread.join();

	::DestroyWindow(Controller::handle);
	return 0;
}

int sparky::win::ControllerGL::Command(int id, int cmd, LPARAM msg)
{
	return 0;
}

int sparky::win::ControllerGL::Create()
{
	if (!m_ViewGL->CreateContext(Controller::handle, 32, 24, 8))
	{
		return -1;
	}

	m_GLthread = std::thread(&ControllerGL::RunThread, this);
	m_LoopFlag = true;

	return 0;
}

int sparky::win::ControllerGL::Destroy()
{
	::PostQuitMessage(0);
	return 0;
}

int sparky::win::ControllerGL::Paint()
{
	return 0;
}

int sparky::win::ControllerGL::LButtonDown(WPARAM state, int x, int y)
{
	return 0;
}

int sparky::win::ControllerGL::LButtonUp(WPARAM state, int x, int y)
{
	return 0;
}

int sparky::win::ControllerGL::RButtonDown(WPARAM state, int x, int y)
{
	return 0;
}

int sparky::win::ControllerGL::RButtonUp(WPARAM state, int x, int y)
{
	return 0;
}

int sparky::win::ControllerGL::MouseMove(WPARAM state, int x, int y)
{
	return 0;
}

int sparky::win::ControllerGL::KeyDown(int key, LPARAM lParam)
{
	return 0;
}

int sparky::win::ControllerGL::Size(int width, int height, WPARAM type)
{
	return 0;
}

void sparky::win::ControllerGL::RunThread()
{
	::wglMakeCurrent(m_ViewGL->GetDC(), m_ViewGL->GetRC());

	m_ModelGL->Init();
	RECT rect;
	::GetClientRect(handle, &rect);
	m_ModelGL->SetViewport(rect.right, rect.bottom);

	while (m_LoopFlag)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		if (m_ResizeFlag)
		{
			m_ModelGL->SetViewport(m_ClientWidth, m_ClientHeight);
			m_ResizeFlag = false;
		}

		m_ModelGL->Draw();
		m_ViewGL->SwapBuffer();
	}

	m_ViewGL->CloseContext(Controller::handle);
	::wglMakeCurrent(0, 0);
}
