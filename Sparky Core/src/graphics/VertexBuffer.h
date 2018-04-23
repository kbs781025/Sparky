#pragma once

#include <GL/glew.h>
#include "BufferLayout.h"
#include "Buffer.h"

namespace sparky { namespace graphics {

	struct VertexBufferContext
	{
		GLenum VBUsage;
		const void* VBData;
		GLuint VBSize;
		BufferLayout VBLayout;

		VertexBufferContext(GLenum Usage, const void* VBarray, GLuint VBsize, BufferLayout Layout)
		{
			VBUsage = Usage;
			VBData = VBarray;
			VBSize = VBsize;
			VBLayout = Layout;
		}

		VertexBufferContext(GLenum Usage, const std::vector<float>& VBvector, const BufferLayout& Layout)
		{
			VBUsage = Usage;
			VBData = VBvector.data();
			VBSize = VBvector.size();
			VBLayout = Layout;
		}
	};

	class VertexBuffer : public Buffer
	{
	private:
		GLuint m_Handle;
		GLenum m_Usage;
		GLuint m_Size; // array size in bytes
		BufferLayout m_Layout;
	public:
		VertexBuffer(GLenum usage, GLuint size, const void* data, const BufferLayout& layout);
		VertexBuffer(const VertexBufferContext& context);
		VertexBuffer(GLenum usage);
		~VertexBuffer();

		void Resize(GLuint size);
		void SetLayout(const BufferLayout& layout);
		void SetData(GLuint size, const void* data);

		void* getInternalPointer();
		void releasePointer();

		virtual void bind() const override;
		virtual void unBind() const override;

		virtual GLuint getHandle() const override;
		inline GLuint getSize() { return m_Size; }
		inline GLuint getCount() { return m_Size / sizeof(GL_FLOAT); }
	private:
		void initVBObject(const void* data);
	};

}}