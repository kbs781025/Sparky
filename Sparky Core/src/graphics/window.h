#pragma once

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "../maths/maths.h"
#include "../graphics/camera.h"

namespace sparky {  namespace graphics {

	#define MAX_KEYS 1024 
	#define MAX_BUTTONS 32 

	class Window
	{
	private:
		const char* m_Title;
		int m_Width, m_Height;
		GLFWwindow* m_Window;
		bool m_Closed;

		bool m_Keys[MAX_KEYS]; // use keycode as an index
		bool m_MouseButtons[MAX_BUTTONS];

		Camera m_Camera;
		float mx, my; // position of mouse
	public:
		float m_MixingRatio;
		float fov;

		Window(const char* title, int width, int height);
		~Window();
		int closed() const;
		void clear() const;
		void update(float delta); // for variable timestep
		void update(); // for fixed timestep
		void processInput(); // for fixed timestep
		void processInput(float delta); // for variable timestep

		inline int getWidth() const { return m_Width; }
		inline int getHeight() const { return m_Height; }
		inline float getFov() const { return fov; }
		inline GLFWwindow* getWindow() const { return m_Window; }
		void zoomCamera(float xoffset, float yoffset);
		glm::mat4 getViewMatrix() const { return m_Camera.getViewMatrix(); }
		glm::vec3 getCamPosition() const { return m_Camera.getCameraPosition(); }
		glm::vec3 getForward() const { return m_Camera.getForward(); }

		bool isKeyPressed(unsigned int keycode) const;
		bool isMouseButtonPressed(unsigned int button) const;
		void getMouseCursorPosition(double& x, double& y) const;
	private:
		bool init();
		friend static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
		friend static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
		friend static void mouse_cursor_callback(GLFWwindow* window, double xpos, double ypos);
		friend static void window_resize(GLFWwindow* window, int width, int height);
		friend static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	};

	}

}