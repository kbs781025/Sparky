#include <cstddef>

#include "../platform/opengl/GLCommon.h"

#include "mesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "shaders.h"

namespace sparky { namespace graphics {

	Mesh::Mesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies, const std::vector<Texture2D>& textures)
		:
		m_pVAO(nullptr),
		m_Textures(textures)
	{
		setupMesh(vertices, layout, indicies);
	}

	void Mesh::Draw(const Shader& shader, bool textureOn) const
	{
		unsigned int diffuseNum = -1;
		unsigned int specularNum = -1;
		unsigned int normalNum = -1;
		shader.enable();
		if (textureOn)
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				std::string type = m_Textures[i].getType();
				std::string number;

				if (type == "texture_diffuse")
				{
					number = std::to_string(++diffuseNum);
				}
				else if (type == "texture_specular")
				{
					number = std::to_string(++specularNum);
				}
				else if (type == "texture_normal")
				{
					number = std::to_string(++normalNum);
				}
				else 
				{
					continue;
				}

				m_Textures[i].bind(i);
				shader.setUniform1i(("material." + type + number).c_str(), i);
			}
		}
		shader.setUniform1f("material.shininess", 256.0);

		m_pVAO->Draw();
	}

	void Mesh::DrawInstances(const Shader & shader, unsigned int instanceCount) const
	{
		unsigned int diffuseNum = 0;
		unsigned int specularNum = 0;
		unsigned int normalNum = 0;
		shader.enable();
		for (int i = 0; i < m_Textures.size(); i++)
		{
			std::string number;
			std::string type = m_Textures[i].getType();

			if (type == "texture_diffuse")
			{
				number = std::to_string(++diffuseNum);
			}
			else if (type == "texture_specular")
			{
				number = std::to_string(++specularNum);
			}
			else if (type == "texture_normal")
			{
				number = std::to_string(++normalNum);
			}

			m_Textures[i].bind(i);
			shader.setUniform1i(("material." + type + number).c_str(), i);
		}

		m_pVAO->DrawInstances(instanceCount);
	}

	// TODO : Implement this method
	void Mesh::Render(const Renderer3D & renderer) const
	{

	}

	unsigned int Mesh::getVAOHandle()
	{
		return m_pVAO->getHandle();
	}

	void Mesh::setupMesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies)
	{
		VertexBufferContext vbContext(GL_STATIC_DRAW, vertices.data(), vertices.size() * sizeof(float), layout);
		IndexBufferContext ibContext(indicies.data(), indicies.size());
		m_pVAO = new VertexArray(vbContext, ibContext);
	}

}
}
