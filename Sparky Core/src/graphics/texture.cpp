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

	Texture::Texture(const std::string & filename, const std::string & textureType, GLuint wrapMethod, GLuint filterMethod)
		:
		m_Path(filename),
		m_Type(textureType)
	{
		loadTexture(filename, wrapMethod, filterMethod);
	}

	Texture::Texture(const std::string& filename, GLuint wrapMethod, GLuint filterMethod)
		:
		m_Path(filename)
	{
		loadTexture(filename, wrapMethod, filterMethod);
	}

	Texture::Texture(int windowWidth, int windowHeight)
	{
		glGenTextures(1, &m_textureID);
		glBindTexture(GL_TEXTURE_2D, m_textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, windowWidth, windowHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Texture::bindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, m_textureID);
	}

	void Texture::unbindTexture() const
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	unsigned int Texture::loadCubeMap(const std::vector<std::string> & cubeMapFiles)
	{
		unsigned int cubeMapID;
		glGenTextures(1, &cubeMapID);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

		for (unsigned int i = 0; i < cubeMapFiles.size(); i++)
		{
			unsigned int width, height, format;
			unsigned char* data = sparky::ImageLoader::load_Image(cubeMapFiles[i].c_str(), &width, &height, &format);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		}

		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

		return cubeMapID;
	}

}
}