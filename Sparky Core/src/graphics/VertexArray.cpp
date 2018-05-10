#include "VertexArray.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::VertexArray::VertexArray(const VertexBufferContext& VBcontext, const IndexBufferContext& IBcontext)
	: m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr), m_Layout(VBcontext.VBLayout)
{
	GLCall(glGenVertexArrays(1, &m_Handle));
	GLCall(glBindVertexArray(m_Handle));
	m_pVertexBuffer = new VertexBuffer(VBcontext);

	SetLayout(VBcontext.VBLayout);

	m_pIndexBuffer = new IndexBuffer(IBcontext);
	GLCall(glBindVertexArray(0));
}

sparky::graphics::VertexArray::VertexArray(const VertexBufferContext & VBcontext)
	: m_pVertexBuffer(nullptr), m_pIndexBuffer(nullptr), m_Layout(VBcontext.VBLayout)
{
	GLCall(glGenVertexArrays(1, &m_Handle));
	GLCall(glBindVertexArray(m_Handle));
	m_pVertexBuffer = new VertexBuffer(VBcontext);
	GLCall(glBindVertexArray(0));
}

sparky::graphics::VertexArray::~VertexArray()
{
	GLCall(glDeleteVertexArrays(1, &m_Handle));
	delete m_pVertexBuffer;
	delete m_pIndexBuffer;
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
}

void sparky::graphics::VertexArray::DrawInstances(GLuint instanceCount)
{
	bind();
	if (m_pIndexBuffer)
	{
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_pIndexBuffer->getCount(), GL_UNSIGNED_BYTE, 0, instanceCount));
	}
	else
	{
		GLCall(glDrawArraysInstanced(GL_TRIANGLES, 0, m_pVertexBuffer->getCount(), instanceCount));
	}
}

void sparky::graphics::VertexArray::SetLayout(const BufferLayout & layout)
{
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