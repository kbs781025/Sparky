#include "window.h"

namespace sparky { namespace graphics {


	Window::Window(const char * title, int width, int height)
	{
		m_Title = title;
		m_Width = width;
		m_Height = height;

		if (!init())
		{
			glfwTerminate();
		}

		for (int i = 0; i < MAX_KEYS; i++)
		{
			m_Keys[i] = false;
		}
		
		for (int i = 0; i < MAX_BUTTONS; i++)
		{
			m_MouseButtons[i] = false;
		}
	}

	Window::~Window()
	{
		glfwTerminate();
	}

	int Window::closed() const
	{
		return glfwWindowShouldClose(m_Window);
	}

	void Window::clear() const
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	void Window::update()
	{
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			std::cout << "OpenGL Error : " << error << std::endl;
		}

		processInput(m_Window);

		glfwPollEvents();
		/*glfwGetFramebufferSize(m_Window, &m_Width, &m_Height);
		glViewport(0, 0, m_Width, m_Height);*/
		glfwSwapBuffers(m_Window);
	}

	bool Window::isKeyPressed(unsigned int keycode) const
	{
		if (keycode >= MAX_KEYS)
		{
			return false;
		}

		return m_Keys[keycode];
	}

	bool Window::isMouseButtonPressed(unsigned int button) const
	{
		return m_MouseButtons[button];
	}

	void Window::getMouseCursorPosition(double & x, double & y) const
	{
		x = mx;
		y = my;
	}

	bool Window::init()
	{
		if (!glfwInit())
		{
			std::cout << "Failed to initialize glfw." << std::endl;
			return false;
		}

		m_Window = glfwCreateWindow(m_Width, m_Height, m_Title, nullptr, nullptr);
		if (!m_Window)
		{
			glfwTerminate();
			std::cout << "Failed to create window." << std::endl;
			return false;
		}

		glfwMakeContextCurrent(m_Window);
		glfwSetWindowUserPointer(m_Window, this); // pass this window class pointer to GL
		glfwSetFramebufferSizeCallback(m_Window, window_resize);
		glfwSetKeyCallback(m_Window, key_callback);
		glfwSetMouseButtonCallback(m_Window, mouse_button_callback);
		glfwSetCursorPosCallback(m_Window, mouse_cursor_callback);
		glfwSwapInterval(0); // 1 vsync on

		if (glewInit() != GLEW_OK)
		{
			std::cout << "Failed to initialize GLEW" << std::endl;
			return false;
		}

		std::cout << glGetString(GL_VERSION) << std::endl;

		return true;
	}

	void Window::processInput(GLFWwindow * window)
	{
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		{
			glfwSetWindowShouldClose(window, true);
		}
	}

	void window_resize(GLFWwindow* window, int width, int height) // not part of window class
	{
		glViewport(0, 0, width, height);
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_Width = width;
		win->m_Height = height;
	}

	void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Window* win = (Window*) glfwGetWindowUserPointer(window);
		win->m_Keys[key] = (action != GLFW_RELEASE);
	}

	void mouse_button_callback(GLFWwindow * window, int button, int action, int mods)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->m_MouseButtons[button] = (action != GLFW_RELEASE);
	}

	void mouse_cursor_callback(GLFWwindow * window, double xpos, double ypos)
	{
		Window* win = (Window*)glfwGetWindowUserPointer(window);
		win->mx = xpos;
		win->my = ypos;
	}


} }
