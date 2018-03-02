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
	private:
		void loadTexture(const std::string& filename);
	public:
		Texture(const std::string& filename);

		GLuint getTextureID() { return m_textureID; }
		void bindTexture() const;
		void unbindTexture() const;
	
		inline const unsigned int getWidth() { return m_Width; }
		inline const unsigned int getHeight() { return m_Height; }
	};

}}