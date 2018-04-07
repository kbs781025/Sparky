#include "texture.h"
#include "../utils/imageloader.h"
#include <iostream>

namespace sparky { namespace graphics {

	void Texture::loadTexture(const std::string& filename, GLuint textureType, GLuint wrapMethod, GLuint filterMethod)
	{
		GLenum format;
		unsigned char* pixels = ImageLoader::load_Image(filename.c_str(), &m_Width, &m_Height, &format);
		if (!pixels)
		{
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(textureType, m_textureID);

		glTexImage2D(textureType, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, wrapMethod);
		glTexParameteri(textureType, GL_TEXTURE_WRAP_T, wrapMethod);
		if (textureType == GL_TEXTURE_CUBE_MAP)
		{
			glTexParameteri(textureType, GL_TEXTURE_WRAP_R, wrapMethod);
		}
		glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, filterMethod);
		glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, filterMethod);

		ImageLoader::free_image(pixels);
		glBindTexture(textureType, 0);
	}

	Texture::Texture(const std::string & filename, const std::string & textureType, GLuint wrapMethod, GLuint filterMethod)
		:
		m_Path(filename),
		m_Type(textureType)
	{
		loadTexture(filename, GL_TEXTURE_2D, wrapMethod, filterMethod);
	}

	Texture::Texture(const std::string& filename, GLuint wrapMethod, GLuint filterMethod)
		:
		m_Path(filename)
	{
		loadTexture(filename, GL_TEXTURE_2D, wrapMethod, filterMethod);
	}

	Texture::Texture(const std::string & filename, GLuint textureType, GLuint wrapMethod, GLuint filterMethod)
		:
		m_Path(filename)
	{
		loadTexture(filename, textureType, wrapMethod, filterMethod);
	}

	void Texture::bindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture::unbindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

}
}