#include "mesh.h"
#include <cstddef>
#include "../platform/opengl/GLCommon.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"

namespace sparky { namespace graphics {

	Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies, const std::vector<Texture2D>& textures)
		:
		m_Vertices(vertices),
		m_Indicies(indicies),
		m_Textures(textures)
	{
		setupMesh(vertices, indicies);
	}

	void Mesh::Draw(Shader& shader, bool textureOn)
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

	void Mesh::DrawInstances(Shader & shader, unsigned int instanceCount)
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

	unsigned int Mesh::getVAOHandle()
	{
		return m_pVAO->getHandle();
	}

	void Mesh::setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies)
	{
		VertexBufferContext vbContext(GL_STATIC_DRAW, m_Vertices.data(), m_Vertices.size() * sizeof(Vertex), BufferLayout::getPosTexTBNLayout());
		IndexBufferContext ibContext(m_Indicies.data(), m_Indicies.size());
		m_pVAO = new VertexArray(vbContext, ibContext);
	}

}
}
