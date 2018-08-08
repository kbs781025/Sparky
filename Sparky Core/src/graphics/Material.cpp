#include "Material.h"

sparky::graphics::Material::Material()
	:
	m_Shader(nullptr)
{
}

sparky::graphics::Material::~Material()
{
	delete m_Shader;
	m_Shader = nullptr;
}

void sparky::graphics::Material::AttachShader(Shader * shader)
{
	m_Shader = shader;
}

void sparky::graphics::Material::AddTexture(TextureType type, const std::string & filepath)
{
	m_Textures[type] = new Texture2D(filepath);
}

void sparky::graphics::Material::Bind() const
{
	for (int type = TextureType::ALBEDO; type <= TextureType::SPECULAR; ++type)
	{
		auto it = m_Textures.find(TextureType(type));
		if (it != m_Textures.end())
		{
			it->second->bind(type);
		}
	}
}

void sparky::graphics::Material::Unbind() const
{
	for (int type = TextureType::ALBEDO; type <= TextureType::SPECULAR; ++type)
	{
		auto it = m_Textures.find(TextureType(type));
		if (it != m_Textures.end())
		{
			it->second->unBind(type);
		}
	}
}

