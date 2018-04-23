#pragma once

#include "texture.h"

namespace sparky { namespace graphics {

	class TextureDepth : public Texture
	{
	private:
		std::string m_Name;
		GLuint m_Handle;
		GLuint m_Width, m_Height;
	public:
		TextureDepth(GLuint width, GLuint height);
		~TextureDepth();

		void bind(GLuint slot = 0) const override;
		void unBind(GLuint slot = 0) const override; 
		virtual GLuint getWidth() const override { return m_Width; }
		virtual GLuint getHeight() const override { return m_Height; }

		inline const std::string& getName() const override { return m_Name; }
		inline const std::string& getFilePath() const override { return m_Name; }
		inline GLuint getHandle() const override { return m_Handle; }
	private:
		GLuint initDepthMap();
	};

}}