#include "TextureCube.h"
#include "Texture2D.h"

sparky::graphics::TextureCube::TextureCube(const std::string & name, std::string & filePath)
	: m_Name(name)
{
	m_Files.push_back(filePath);
	m_Handle = loadFromSingleFile();
}

sparky::graphics::TextureCube::TextureCube(const std::string & name, std::vector<std::string>& files)
	: m_Name(name), m_Files(files)
{
	m_Handle = loadFromMultipleFiles();
}

sparky::graphics::TextureCube::~TextureCube()
{
	glDeleteTextures(1, &m_Handle);
}

void sparky::graphics::TextureCube::bind(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Handle);
}

void sparky::graphics::TextureCube::unBind(GLuint slot) const
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

GLuint sparky::graphics::TextureCube::loadFromSingleFile()
{
	// TODO : implement
	GLubyte* pixels = nullptr;
	return 0; 
}

GLuint sparky::graphics::TextureCube::loadFromMultipleFiles()
{
	unsigned int cubeMapID;
	glGenTextures(1, &cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);

	GLubyte* pixels = nullptr;

	for (unsigned int i = 0; i < m_Files.size(); i++)
	{
		int bytes;
		pixels = ImageLoader::load_Image(m_Files[i].c_str(), &m_Width, &m_Height, &bytes);
		if (bytes != 3 && bytes != 4)
		{
			std::cout << "[Texture] Unsupported Image format" << std::endl;
		}
		m_Parameters.format = bytes == 3 ? TextureFormat::RGB : TextureFormat::RGBA;
		GLenum format = Texture2D::textureFormatToGL(m_Parameters.format);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, format, m_Width, m_Height, 0, format, GL_UNSIGNED_BYTE, pixels);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	
	return cubeMapID;
}
