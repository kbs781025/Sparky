#include "Framebuffer2D.h"

sparky::graphics::FrameBuffer2D::FrameBuffer2D(GLuint width, GLuint height)
	: m_Width(width), m_Height(height)
{
}

sparky::graphics::FrameBuffer2D::~FrameBuffer2D()
{
	delete m_pTexture;
	m_pTexture = nullptr;
	glDeleteFramebuffers(1, &m_FrameBufferHandle);
}

void sparky::graphics::FrameBuffer2D::bind() const
{
	glViewport(0, 0, m_Width, m_Height);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferHandle);
}

void sparky::graphics::FrameBuffer2D::unBind() const
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GLuint sparky::graphics::FrameBuffer2D::getHandle() const
{
	return m_FrameBufferHandle;
}

void sparky::graphics::FrameBuffer2D::clear()
{
	glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void sparky::graphics::FrameBuffer2D::initFrameBuffer()
{
	glGenFramebuffers(1, &m_FrameBufferHandle);
	glGenRenderbuffers(1, &m_DepthBufferHandle);

	TextureParameters parameter;
	m_pTexture = new Texture2D(m_Width, m_Height, parameter);

	glBindRenderbuffer(GL_RENDERBUFFER, m_DepthBufferHandle);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, m_Width, m_Height);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FrameBufferHandle);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_pTexture->getHandle(), 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, m_DepthBufferHandle);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
