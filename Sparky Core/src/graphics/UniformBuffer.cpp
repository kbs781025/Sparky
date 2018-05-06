#include "UniformBuffer.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::UniformBuffer::UniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize)
	:
	m_BlockSize(uniformBlockSize)
{
	initUniformBuffer(bindingPoint, m_BlockSize);
}

sparky::graphics::UniformBuffer::UniformBuffer(const std::vector<float>& uniformBlockData, GLuint bindingPoint = 0)
	:
	m_BlockSize(uniformBlockData.size() * sizeof(float))
{
	initUniformBuffer(bindingPoint, m_BlockSize, uniformBlockData.data());
}

sparky::graphics::UniformBuffer::~UniformBuffer()
{
	GLCall(glDeleteBuffers(1, &m_Handle));
}

void sparky::graphics::UniformBuffer::bind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
}

void sparky::graphics::UniformBuffer::unBind() const
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

GLuint sparky::graphics::UniformBuffer::getHandle() const
{
	return m_Handle;
}

void sparky::graphics::UniformBuffer::setBindingPoint(GLuint bindingPoint)
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Handle));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void sparky::graphics::UniformBuffer::setData(const void * data)
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, m_BlockSize, data));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void sparky::graphics::UniformBuffer::setDataSegment(GLuint uniformOffset, const void * data, GLuint byteSize)
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, uniformOffset, byteSize * sizeof(float), data));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void sparky::graphics::UniformBuffer::setData(const std::vector<float>& uniformBlockData)
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	_ASSERT(uniformBlockData.size() * sizeof(float) == m_BlockSize);
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, 0, m_BlockSize, uniformBlockData.data()));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void sparky::graphics::UniformBuffer::setDataSegment(GLuint uniformOffset, const std::vector<float>& uniformBlockData)
{
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	GLCall(glBufferSubData(GL_UNIFORM_BUFFER, uniformOffset, uniformBlockData.size() * sizeof(float), uniformBlockData.data()));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void sparky::graphics::UniformBuffer::initUniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize)
{
	GLCall(glGenBuffers(1, &m_Handle));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, nullptr, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Handle));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}

void sparky::graphics::UniformBuffer::initUniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize, const void * data)
{
	GLCall(glGenBuffers(1, &m_Handle));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, m_Handle));
	GLCall(glBufferData(GL_UNIFORM_BUFFER, uniformBlockSize, data, GL_DYNAMIC_DRAW));
	GLCall(glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Handle));
	GLCall(glBindBuffer(GL_UNIFORM_BUFFER, 0));
}
