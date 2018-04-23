#pragma once

#include <GL/glew.h>
#include "Buffer.h"
#include "TextureDepth.h"

namespace sparky { namespace graphics {

	class FrameBufferDepth : public Buffer
	{
	private:
		GLuint m_Handle;
		GLuint m_Width, m_Height;
		TextureDepth * m_pDepthMap;
	public:
		FrameBufferDepth(GLuint width, GLuint height);
		~FrameBufferDepth();
		virtual void bind() const override;
		virtual void unBind() const override;
		virtual GLuint getHandle() const override;
		void clear() const;
		inline TextureDepth* getTextureDepth() const { return m_pDepthMap; }
	private:
		void initFrameBuffer();
	};

}}