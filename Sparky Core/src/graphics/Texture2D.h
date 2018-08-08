#pragma once

#include "texture.h"
#include "../utils/imageloader.h"
#include <iostream>

namespace sparky { namespace graphics {

	class Texture2D : public Texture
	{
	private:
		std::string m_FileName;
		std::string m_TypeName;
		GLuint m_Handle;
		GLuint m_Width, m_Height;
		TextureParameters m_Parameters;
	public:
		Texture2D(const std::string& fileName, const TextureParameters& parameters = TextureParameters());
		Texture2D(GLuint width, GLuint height, const TextureParameters& parameters = TextureParameters());
		~Texture2D();

		void bind(GLuint slot = 0) const override;
		void unBind(GLuint slot = 0) const override;

		virtual GLuint getWidth() const override { return m_Width; }
		virtual GLuint getHeight() const override { return m_Height; }

		void setData(const void* pixels);
		void setData(GLuint color);

		inline const std::string& getFilePath() const override { return m_FileName; }
		inline GLuint getHandle() const override { return m_Handle; }
		inline const std::string& getType() const { return m_TypeName; }
	private:
		GLuint loadTexture();
	public:
		static GLuint textureFormatToGL(TextureFormat format);
		static GLuint textureWrapToGL(TextureWrap wrap);
	};

} }