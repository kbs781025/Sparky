#include "ModelGL.h"
#include "../../maths/maths.h"
#include <GL/glew.h>

sparky::opengl::ModelGL::ModelGL()
	:
	m_MouseLeftDown(false), m_MouseRightDown(false),
	m_MouseX(0), m_MouseY(0), m_Camera(nullptr)
{
}

sparky::opengl::ModelGL::~ModelGL()
{
	delete m_Camera;
	m_Camera = nullptr;
}

void sparky::opengl::ModelGL::Init()
{
	//m_Camera = new graphics::Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f),
//			glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f));
}

void sparky::opengl::ModelGL::SetCamera(float posX, float posY, float posZ, float targetX, float targetY, float targetZ)
{
}

void sparky::opengl::ModelGL::SetViewport(int width, int height)
{
	glViewport(0, 0, (unsigned int)width, (unsigned int)height);
	//TODO : set new projection matrix
}

void sparky::opengl::ModelGL::Draw()
{
	for (const auto& model : m_Models)
	{
		model.second->Draw();
	}
}

void sparky::opengl::ModelGL::RotateCamera(int x, int y)
{
	if (m_MouseLeftDown)
	{
		m_Camera->rotateCamera(x - m_MouseX, y - m_MouseY);
		m_MouseX = x; 
		m_MouseY = y;
	}
}

void sparky::opengl::ModelGL::ZoomCamera(int dist)
{
	if (m_MouseRightDown)
	{
		//TODO : implement camera zoom
	}
}
