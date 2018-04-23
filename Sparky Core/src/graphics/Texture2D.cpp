#include "Texture2D.h"

sparky::graphics::Texture2D::Texture2D(const std::string& name, const std::string& fileName, const TextureParameters& parametres)
	: m_Name(name), m_FileName(fileName), m_Parameters(m_Parameters)
{
	m_Handle = loadTexture();
}

sparky::graphics::Texture2D::~Texture2D()
{
	glDeleteTextures(1, &m_Handle);
}

void sparky::graphics::Texture2D::bind(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_Handle);
}

void sparky::graphics::Texture2D::unBind(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void sparky::graphics::Texture2D::setData(const void * pixels)
{
	glBindTexture(GL_TEXTURE_2D, m_Handle);
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_Width, m_Height, textureFormatToGL(m_Parameters.format), GL_UNSIGNED_BYTE, pixels);
}

void sparky::graphics::Texture2D::setData(GLuint color)
{
}

GLuint sparky::graphics::Texture2D::loadTexture()
{
	GLubyte* pixels = nullptr;
	if (m_FileName != "NULL")
	{
		int bytes;
		pixels = ImageLoader::load_Image(m_FileName.c_str(), &m_Width, &m_Height, &bytes);
		if (bytes != 3 && bytes != 4)
		{
			std::cout << "[Texture] Unsupported Image format" << std::endl;
		}
		m_Parameters.format = bytes == 3 ? TextureFormat::RGB : TextureFormat::RGBA;
	}

	GLuint handle;
	glGenTextures(1, &handle);
	glBindTexture(GL_TEXTURE_2D, handle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Parameters.filter == TextureFilter::LINEAR ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Parameters.filter == TextureFilter::LINEAR ? GL_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrapToGL(m_Parameters.wrap));
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrapToGL(m_Parameters.wrap));

	GLenum format = textureFormatToGL(m_Parameters.format);
	glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixels ? pixels : nullptr);
	glGenerateMipmap(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, 0);

	if (pixels != nullptr)
		ImageLoader::free_image(pixels);

	return handle;
}

GLuint sparky::graphics::Texture2D::textureFormatToGL(TextureFormat format)
{
	switch (format)
	{
	case TextureFormat::RGBA:				return GL_RGBA;
	case TextureFormat::RGB:				return GL_RGB;
	case TextureFormat::LUMINANCE:			return GL_LUMINANCE;
	case TextureFormat::LUMINANCE_ALPHA:	return GL_LUMINANCE_ALPHA;
	}
	return 0;
}

GLuint sparky::graphics::Texture2D::textureWrapToGL(TextureWrap wrap)
{
	switch (wrap)
	{
	case TextureWrap::CLAMP:			return GL_CLAMP;
	case TextureWrap::CLAMP_TO_BORDER:	return GL_CLAMP_TO_BORDER;
	case TextureWrap::CLAMP_TO_EDGE:	return GL_CLAMP_TO_EDGE;
	case TextureWrap::REPEAT:			return GL_REPEAT;
	case TextureWrap::MIRRORED_REPEAT:	return GL_MIRRORED_REPEAT;
	}
	return 0;
}
