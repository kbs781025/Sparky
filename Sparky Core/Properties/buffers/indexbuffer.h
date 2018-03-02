#pragma once

#include <GL/glew.h>

namespace sparky { namespace graphics {

	class IndexBuffer
	{
	private:
		GLuint m_BufferID;
		GLuint m_Count;
	public:
		IndexBuffer(GLushort* data, GLsizei count); // ushort 0 ~ 65535(2 ^ 16)
		~IndexBuffer();

		void bind() const;
		void unbind() const;

		inline GLuint getCount() const { return m_Count; }
	};

	}
}