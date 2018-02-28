#pragma once

#include <string>
#include <GL/glew.h>
#include <FreeImage.h>
#include "../utils/imageloader.h"
#include "TextureManager.h"

namespace sparky { namespace graphics {

	class Texture
	{
	private:
		std::string m_Path;
		GLuint m_TextureID;
		GLsizei m_Width, m_Height;
	public:
		Texture(const std::string& filePath);
		~Texture();

		void bind() const;
		void unbind() const;

		inline GLuint getWidth() { return m_Width; }
		inline GLuint getHeight() { return m_Height; }
		inline GLuint getTextureID() { return m_TextureID; }
	private:
		void load();

	};

}}