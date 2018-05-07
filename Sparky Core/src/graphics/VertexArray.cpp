#include "VertexArray.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::VertexArray::VertexArray(const VertexBufferContext& VBcontext, const IndexBufferContext& IBcontext)
	: m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr)
{
	GLCall(glGenVertexArrays(1, &m_Handle));
	GLCall(glBindVertexArray(m_Handle));
	m_pVertexBuffer = new VertexBuffer(VBcontext);
	m_pIndexBuffer = new IndexBuffer(IBcontext);
	GLCall(glBindVertexArray(0));
}

sparky::graphics::VertexArray::VertexArray(const VertexBufferContext & VBcontext)
	: m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr)
{
	GLCall(glGenVertexArrays(1, &m_Handle));
	GLCall(glBindVertexArray(m_Handle));
	m_pVertexBuffer = new VertexBuffer(VBcontext);
	GLCall(glBindVertexArray(0));
}

sparky::graphics::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_Handle));
}

void sparky::graphics::VertexArray::bind() const
{
	GLCall(glBindVertexArray(m_Handle));
}

void sparky::graphics::VertexArray::unBind() const
{
	GLCall(glBindVertexArray(0));
}

void sparky::graphics::VertexArray::Draw()
{
	bind();
	if (m_pIndexBuffer)
	{
		GLCall(glDrawElements(GL_TRIANGLES, m_pIndexBuffer->getCount(), GL_UNSIGNED_BYTE, 0));
	}
	else
	{
		GLCall(glDrawArrays(GL_TRIANGLES, 0, m_pVertexBuffer->getCount()));
	}
	unBind();
}
