#include "FrameBufferDepth.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::FrameBufferDepth::FrameBufferDepth(GLuint width, GLuint height)
	: m_Width(width), m_Height(height)
{
	initFrameBuffer();
}

sparky::graphics::FrameBufferDepth::~FrameBufferDepth()
{
	delete m_pDepthMap;
	m_pDepthMap = nullptr;
	GLCall(glDeleteFramebuffers(1, &m_Handle));
}

void sparky::graphics::FrameBufferDepth::bind() const
{
	GLCall(glViewport(0, 0, m_Width, m_Height)); 
	GLCall(glCullFace(GL_FRONT));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Handle));
	clear();
}

void sparky::graphics::FrameBufferDepth::unBind() const
{
	//GLCall(glViewport(0, 0, window.width, window.height)); To DO : Implement
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	GLCall(glCullFace(GL_BACK));
}

GLuint sparky::graphics::FrameBufferDepth::getHandle() const
{
	return m_Handle;
}

void sparky::graphics::FrameBufferDepth::clear() const
{
	GLCall(glClearColor(1.0f, 1.0f, 1.0f, 1.0f));
	GLCall(glClear(GL_DEPTH_BUFFER_BIT));
}

void sparky::graphics::FrameBufferDepth::initFrameBuffer()
{
	GLCall(glGenFramebuffers(1, &m_Handle));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, m_Handle));
	m_pDepthMap = new TextureDepth(m_Width, m_Height);
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_pDepthMap->getHandle(), 0));
	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}
