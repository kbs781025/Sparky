#pragma once

#include <GL/glew.h>
#include "Buffer.h"
#include "Texture2D.h"
#include "../maths/maths.h"

namespace sparky { namespace graphics {

	class FrameBuffer2D : public Buffer
	{
	private:
		GLuint m_FrameBufferHandle;
		GLuint m_DepthBufferHandle;
		GLuint m_Width, m_Height;
		Texture2D * m_pTexture;
		glm::vec4 m_ClearColor;
	public:
		FrameBuffer2D(GLuint width, GLuint height);
		~FrameBuffer2D();

		virtual void bind() const override;
		virtual void unBind() const override;
		virtual GLuint getHandle() const override;
		// Should be virtual later : Buffer -> FrameBuffer -> FrameBuffer2D and FrameBufferDepth
		void clear();
		inline GLuint getWidth() const { return m_Width; }
		inline GLuint getHeight() const { return m_Height; }
		inline Texture2D* getTexture() const { return m_pTexture; }

		inline void setClearColor(const glm::vec4 clearcolor) { m_ClearColor = clearcolor; }
	private:
		void initFrameBuffer();
	};

}}