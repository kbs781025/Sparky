#include "ModelGL.h"
#include "../../graphics/model.h"
#include <GL/glew.h>

sparky::opengl::ModelGL::ModelGL()
	:
	m_MouseLeftDown(false), m_MouseRightDown(false),
	m_MouseX(0), m_MouseY(0), 
	m_ShaderSet(), m_Camera(nullptr), m_ProjectionMat(glm::mat4())
{
}

sparky::opengl::ModelGL::~ModelGL()
{
	delete m_Camera;
	m_Camera = nullptr;
}

void sparky::opengl::ModelGL::Init()
{
	m_Camera = new graphics::Camera(glm::vec3(0.0f, 0.0f, 5.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ProjectionMat = glm::perspective(glm::radians(45.0f), 800 / 600.0f, 0.1f, 100.0f);

	graphics::Model* pModel = new graphics::Model("res/models/man/muro.obj");
	m_Models["Muro"] = pModel;
}

void sparky::opengl::ModelGL::SetViewport(int width, int height)
{
	glViewport(0, 0, (unsigned int)width, (unsigned int)height);
	m_ProjectionMat = glm::perspective(glm::radians(45.0f), (float)width / height, 0.1f, 100.0f);
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
