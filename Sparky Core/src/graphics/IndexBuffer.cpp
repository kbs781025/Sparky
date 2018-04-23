#include "IndexBuffer.h"

sparky::graphics::IndexBuffer::IndexBuffer(const void* data, GLuint count)
	: m_Count(count)
{
	initIBObject(data);
}

sparky::graphics::IndexBuffer::IndexBuffer(const IndexBufferContext & context)
	: m_Count(context.Count)
{
	initIBObject(context.IBData);
}

sparky::graphics::IndexBuffer::~IndexBuffer()
{
	glDeleteBuffers(1, &m_Handle);
}

void sparky::graphics::IndexBuffer::bind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
}

void sparky::graphics::IndexBuffer::unBind() const
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint sparky::graphics::IndexBuffer::getHandle() const
{
	return m_Handle;
}

void sparky::graphics::IndexBuffer::initIBObject(const void * data)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Handle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(GLuint), data, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

