#include "GBuffer.h"
#include "../platform/opengl/GLCommon.h"
#include <stdio.h>

Sparky::Graphics::GBuffer::GBuffer()
{
}

Sparky::Graphics::GBuffer::~GBuffer()
{
}

bool Sparky::Graphics::GBuffer::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	GLCall(glGenFramebuffers(1, &m_FBO));
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO));

	GLCall(glGenTextures(GBUFFER_NUM_TEXTURES, m_Textures));
	GLCall(glGenTextures(1, &m_DepthTexture));

	for (unsigned int i = 0; i < GBUFFER_NUM_TEXTURES; ++i)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, m_Textures[i]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, WindowWidth, WindowHeight, 0, GL_RGB, GL_FLOAT, nullptr));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, m_Textures[i], 0));
	}

	GLCall(glBindTexture(GL_TEXTURE_2D, m_DepthTexture));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_DepthTexture, 0));

	GLenum DrawBuffers[] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
	GLCall(glDrawBuffers(4, DrawBuffers));

	GLenum Status = GLCall(glCheckFramebufferStatus(GL_FRAMEBUFFER));

	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error, status : 0x%x\n", Status);
		return false;
	}

	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0));

	return false;
}

void Sparky::Graphics::GBuffer::BindForWriting()
{
	GLCall(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_FBO));
}

void Sparky::Graphics::GBuffer::BindForReading()
{
	GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_FBO));
}

void Sparky::Graphics::GBuffer::SetReadBuffer(GBUFFER_TEXTURE_TYPE TextureType)
{
	GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0 + TextureType));
}
