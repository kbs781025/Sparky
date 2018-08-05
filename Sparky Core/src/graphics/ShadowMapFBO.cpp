#include "ShadowMapFBO.h"
#include "../platform/opengl/GLCommon.h"
#include <cstdio>

sparky::graphics::ShadowMapFBO::ShadowMapFBO()
{
}

sparky::graphics::ShadowMapFBO::~ShadowMapFBO()
{
}

bool sparky::graphics::ShadowMapFBO::Init(unsigned int WindowWidth, unsigned int WindowHeight)
{
	GLCall(glGenFramebuffers(1, &m_FBO));

	GLCall(glGenTextures(1, &m_ShadowMap));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ShadowMap));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, WindowWidth, WindowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
	GLCall(glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMap, 0));

	GLCall(glDrawBuffer(GL_NONE));
	GLCall(glReadBuffer(GL_NONE));

	GLCall(GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER));
	if (Status != GL_FRAMEBUFFER_COMPLETE)
	{
		printf("FB error. status : 0x%x\n", Status);
		return false;
	}
	return true;
}

void sparky::graphics::ShadowMapFBO::BindForWriting()
{
	GLCall(glBindFramebuffer(GL_DRAW_BUFFER, m_FBO));
}

void sparky::graphics::ShadowMapFBO::BindForReading(GLenum TextureUnit)
{
	GLCall(glActiveTexture(TextureUnit));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_ShadowMap));
}
