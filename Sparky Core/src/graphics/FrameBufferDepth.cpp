#include "FrameBufferDepth.h"

sparky::graphics::FrameBufferDepth::FrameBufferDepth(GLuint width, GLuint height)
	: m_Width(width), m_Height(height)
{
	initFrameBuffer();
}

sparky::graphics::FrameBufferDepth::~FrameBufferDepth()
{
	delete m_pDepthMap;
	m_pDepthMap = nullptr;
	glDeleteFramebuffers(1, &m_Handle);
}

void sparky::graphics::FrameBufferDepth::bind() const
{
	glViewport(0, 0, m_Width, m_Height); 
	glCullFace(GL_FRONT);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
}

void sparky::graphics::FrameBufferDepth::unBind() const
{
	//glViewport(0, 0, window.width, window.height); To DO : Implement
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glCullFace(GL_BACK);
}

GLuint sparky::graphics::FrameBufferDepth::getHandle() const
{
	return m_Handle;
}

void sparky::graphics::FrameBufferDepth::clearDepth() const
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
}

void sparky::graphics::FrameBufferDepth::initFrameBuffer()
{
	glGenFramebuffers(1, &m_Handle);
	glBindFramebuffer(GL_FRAMEBUFFER, m_Handle);
	m_pDepthMap = new TextureDepth(m_Width, m_Height);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pDepthMap->getHandle(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
