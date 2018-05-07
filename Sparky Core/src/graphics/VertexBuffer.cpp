#include "VertexBuffer.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::VertexBuffer::VertexBuffer(GLenum usage, GLuint size, const void* data, const BufferLayout& layout)
	: m_Usage(usage), m_Size(size), m_Layout(layout)
{
	initVBObject(data);
}

sparky::graphics::VertexBuffer::VertexBuffer(const VertexBufferContext & context)
	: m_Usage(context.VBUsage), m_Size(context.VBSize), m_Layout(context.VBLayout)
{
	initVBObject(context.VBData);
}

sparky::graphics::VertexBuffer::VertexBuffer(GLenum usage)
	: m_Size(0), m_Usage(usage)
{
	GLCall(glGenBuffers(1, &m_Handle));
}

sparky::graphics::VertexBuffer::~VertexBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Handle));
}

void sparky::graphics::VertexBuffer::Resize(GLuint size)
{
	m_Size = size;
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, nullptr, m_Usage));
}

void sparky::graphics::VertexBuffer::SetLayout(const BufferLayout & layout)
{
	m_Layout = layout;

	const std::vector<BufferElement>& elements = layout.getLayOut();
	GLuint offset = 0;
	for (unsigned int index = 0; index < elements.size(); index++)
	{
		const BufferElement& element = elements[index];
		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(index, element.Count, element.Type, element.Normalize, layout.getStride(), (void*)offset));
		offset += elements[index].Size;
	}
}

void sparky::graphics::VertexBuffer::SetData(GLuint size, const void * data)
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, m_Usage));
}

void * sparky::graphics::VertexBuffer::getInternalPointer()
{
	void* result = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	return result;
}

void sparky::graphics::VertexBuffer::releasePointer()
{
	GLCall(glUnmapBuffer(GL_ARRAY_BUFFER));
}

void sparky::graphics::VertexBuffer::bind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
}

void sparky::graphics::VertexBuffer::unBind() const
{
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}

GLuint sparky::graphics::VertexBuffer::getHandle() const
{
	return m_Handle;
}

void sparky::graphics::VertexBuffer::initVBObject(const void * data)
{
	GLCall(glGenBuffers(1, &m_Handle));
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_Handle));
	GLCall(glBufferData(GL_ARRAY_BUFFER, m_Size, data, m_Usage));

	const std::vector<BufferElement>& elements = m_Layout.getLayOut();
	GLuint offset = 0;
	for (unsigned int index = 0; index < elements.size(); index++)
	{
		const BufferElement& element = elements[index];
		GLCall(glEnableVertexAttribArray(index));
		GLCall(glVertexAttribPointer(index, element.Count, element.Type, element.Normalize, m_Layout.getStride(), (void*)offset));
		offset += elements[index].Size;
	}
}

