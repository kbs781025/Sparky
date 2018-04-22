#pragma once

#include "texture.h"
#include <vector>
namespace sparky { namespace graphics {

	class TextureCube : public Texture
	{
	private:
		GLuint m_Handle;
		GLuint m_Width, m_Height;
		std::string m_Name;
		std::vector<std::string> m_Files;
		GLuint m_Bits;
		TextureParameters m_Parameters;
	public:
		TextureCube(const std::string& name, std::string& filePath);
		TextureCube(const std::string& name, std::vector<std::string>& files);
		~TextureCube();

		void bind(GLuint slot = 0) const override;
		void unBind(GLuint slot = 0) const override;

		inline const std::string& getName() const override { return m_Name; }
		inline const std::string& getFilePath() const override { return m_Files[0]; }
		inline GLuint getHandle() const override { return m_Handle; }
	private:
		GLuint loadFromSingleFile();
		GLuint loadFromMultipleFiles();
	};

}}
