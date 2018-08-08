#pragma once

#include "shaders.h"
#include "Texture2D.h"
#include <map>
#include <string>

namespace sparky { namespace graphics {

	enum TextureType
	{
		ALBEDO,
		SPECULAR,
		NORMAL
	};

	class Material
	{
	public:
		Material();
		~Material();

		void AttachShader(Shader* shader);
		void AddTexture(TextureType type, const std::string& filepath);
		void Bind() const;
		void Unbind() const;

		unsigned int GetTextureCount() const { return m_Textures.size(); }
		const Texture2D* GetTexture(TextureType type) const { return m_Textures.find(type)->second; } // TODO : m_Textures[type]??
		const Shader* GetShader() const { return m_Shader; }

	private:
		std::map<TextureType, Texture2D*> m_Textures;
		Shader* m_Shader;
	};

} }