#pragma once

#include <GL/glew.h>
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace sparky { namespace graphics {

	class VertexArray
	{
	private:
		GLuint m_Handle;
		VertexBuffer* m_pVertexBuffer;
		IndexBuffer* m_pIndexBuffer;
		BufferLayout m_Layout;
	public:
		VertexArray(const VertexBufferContext& VBcontext, const IndexBufferContext& IBcontext);
		VertexArray(const VertexBufferContext& VBcontext);
		~VertexArray();

		void bind() const;
		void unBind() const;

		void SetLayout(const BufferLayout& layout);

		void Draw();
		void DrawInstances(GLuint instanceCount);

		inline GLuint getHandle() { return m_Handle; }
	};

}}