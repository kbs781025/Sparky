#include "TextureDepth.h"
#include "../platform/opengl/GLCommon.h"

sparky::graphics::TextureDepth::TextureDepth(GLuint width, GLuint height)
	: m_Width(width), m_Height(height)
{
	m_Handle = initDepthMap();
}

sparky::graphics::TextureDepth::~TextureDepth()
{
	GLCall(glDeleteTextures(1, &m_Handle));
}

void sparky::graphics::TextureDepth::bind(GLuint slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, m_Handle));
}

void sparky::graphics::TextureDepth::unBind(GLuint slot) const
{
	GLCall(glActiveTexture(GL_TEXTURE0 + slot));
	GLCall(glBindTexture(GL_TEXTURE_2D, 0));
}

GLuint sparky::graphics::TextureDepth::initDepthMap()
{
	GLuint depthMapID;

	GLCall(glGenTextures(1, &depthMapID));

	GLCall(glBindTexture(GL_TEXTURE_2D, depthMapID));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_Width, m_Height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE, GL_COMPARE_REF_TO_TEXTURE));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC, GL_LEQUAL));
	float borderColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	GLCall(glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor));

	GLCall(glBindTexture(GL_TEXTURE_2D, 0));

	return depthMapID;
}
