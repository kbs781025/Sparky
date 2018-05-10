#include "IndexBuffer.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::IndexBuffer::IndexBuffer(const void* data, GLuint count)
	:
	m_Handle(0),
	m_Count(count)
{
	initIBObject(data);
}

sparky::graphics::IndexBuffer::IndexBuffer(const IndexBufferContext & context)
	:
	m_Handle(0),
	m_Count(context.Count)
{
	initIBObject(context.IBData);
}

sparky::graphics::IndexBuffer::~IndexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Handle));
}

void sparky::graphics::IndexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
}

void sparky::graphics::IndexBuffer::unBind() const
{
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

GLuint sparky::graphics::IndexBuffer::getHandle() const
{
	return m_Handle;
}

void sparky::graphics::IndexBuffer::initIBObject(const void * data)
{
	GLCall(glGenBuffers(1, &m_Handle));
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(GLuint), data, GL_STATIC_DRAW));
}

