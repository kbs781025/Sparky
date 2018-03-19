#pragma once

#include <GL/glew.h>
#include <string>

namespace sparky { namespace graphics {

	class Texture
	{
	private:
		std::string m_Path;
		GLuint m_textureID;
		GLuint m_Width, m_Height;
		GLuint m_WrapAndFilter[2];
	private:
		void loadTexture(const std::string& filename, GLuint wrapMethod, GLuint filterMethod);
	public:
		Texture(const std::string& filename, GLuint wrapMethod = GL_REPEAT, GLuint filterMethod = GL_NEAREST);

		GLuint getTextureID() { return m_textureID; }
		void bindTexture() const;
		void unbindTexture() const;
	
		inline const unsigned int getWidth() { return m_Width; }
		inline const unsigned int getHeight() { return m_Height; }
	};

}}