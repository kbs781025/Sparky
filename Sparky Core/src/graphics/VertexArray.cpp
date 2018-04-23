#include "VertexArray.h"

sparky::graphics::VertexArray::VertexArray(const VertexBufferContext& VBcontext, const IndexBufferContext& IBcontext)
	: m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr)
{
	glGenVertexArrays(1, &m_Handle);
	glBindVertexArray(m_Handle);
	m_pVertexBuffer = new VertexBuffer(VBcontext);
	m_pIndexBuffer = new IndexBuffer(IBcontext);
	glBindVertexArray(0);
}

sparky::graphics::VertexArray::VertexArray(const VertexBufferContext & VBcontext)
	: m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr)
{
	glGenVertexArrays(1, &m_Handle);
	glBindVertexArray(m_Handle);
	m_pVertexBuffer = new VertexBuffer(VBcontext);
	glBindVertexArray(0);
}

sparky::graphics::VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_Handle);
}

void sparky::graphics::VertexArray::bind() const
{
	glBindVertexArray(m_Handle);
}

void sparky::graphics::VertexArray::unBind() const
{
	glBindVertexArray(0);
}

void sparky::graphics::VertexArray::Draw()
{
	bind();
	if (m_pIndexBuffer)
	{
		glDrawElements(GL_TRIANGLES, m_pIndexBuffer->getCount(), GL_UNSIGNED_BYTE, 0);
	}
	else
	{
		glDrawArrays(GL_TRIANGLES, 0, m_pVertexBuffer->getCount());
	}
	unBind();
}
