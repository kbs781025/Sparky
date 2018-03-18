#include "texture.h"
#include "../utils/imageloader.h"
#include <iostream>

namespace sparky { namespace graphics {

	void Texture::loadTexture(const std::string& filename)
	{
		BYTE* pixels = ImageLoader::load_Image(filename.c_str(), &m_Width, &m_Height);
		if (!pixels)
		{
			return;
		}

		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, pixels);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	Texture::Texture(const std::string& filename)
		:
		m_Path(filename)
	{
		loadTexture(filename);
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