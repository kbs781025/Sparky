#include "texture.h"
#include "../utils/imageloader.h"
#include <iostream>

namespace sparky { namespace graphics {

	void Texture::loadTexture(const std::string& filename, GLuint wrapMethod, GLuint filterMethod)
	{
		GLenum format;
		unsigned char* pixels = ImageLoader::load_Image(filename.c_str(), &m_Width, &m_Height, &format);
		if (!pixels)
		{
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMethod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMethod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMethod);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMethod);

		ImageLoader::free_image(pixels);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(const std::string& filename, GLuint wrapMethod, GLuint filterMethod)
		:
		m_Path(filename)
	{
		loadTexture(filename, wrapMethod, filterMethod);
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