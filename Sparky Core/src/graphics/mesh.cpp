#include "mesh.h"
#include <cstddef>
namespace sparky { namespace graphics {

	Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture2D> textures)
		:
		m_Vertices(vertices),
		m_Indicies(indicies),
		m_Textures(textures)
	{
		setupMesh();
	}

	void Mesh::Draw(Shader& shader, bool textureOn)
	{
		unsigned int diffuseNum = 0;
		unsigned int specularNum = 0;
		unsigned int reflectNum = 0;
		shader.enable();
		if (textureOn)
		{
			for (int i = 0; i < m_Textures.size(); i++)
			{
				std::string number;
				//std::string type = m_Textures[i].getType();
				std::string type = "";

				if (type == "texture_diffuse")
				{
					number = std::to_string(++diffuseNum);
				}
				else if (type == "texture_specular")
				{
					number = std::to_string(++specularNum);
				}
				else if (type == "texture_reflect")
				{
					number = std::to_string(++reflectNum);
				}

				m_Textures[i].bind(i);
				shader.setUniform1i(("material." + type + number).c_str(), i);
			}
		}

		glBindVertexArray(m_VAO);
		glDrawElements(GL_TRIANGLES, m_Indicies.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::DrawInstances(Shader & shader, unsigned int instanceCount)
	{
		unsigned int diffuseNum = 0;
		unsigned int specularNum = 0;
		unsigned int reflectNum = 0;
		shader.enable();
		for (int i = 0; i < m_Textures.size(); i++)
		{
			std::string number;
			//std::string type = m_Textures[i].getType();
			std::string type = "";

			if (type == "texture_diffuse")
			{
				number = std::to_string(++diffuseNum);
			}
			else if (type == "texture_specular")
			{
				number = std::to_string(++specularNum);
			}
			else if (type == "texture_reflect")
			{
				number = std::to_string(++reflectNum);
			}

			m_Textures[i].bind(i);
			shader.setUniform1i(("material." + type + number).c_str(), i);
		}

		glBindVertexArray(m_VAO);
		glDrawElementsInstanced(GL_TRIANGLES, m_Indicies.size(), GL_UNSIGNED_INT, 0, instanceCount);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

	void Mesh::setupMesh()
	{
		glGenVertexArrays(1, &m_VAO);
		glGenBuffers(1, &m_VBO);
		glGenBuffers(1, &m_EBO);

		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW);
		
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicies.size() * sizeof(unsigned int), m_Indicies.data(), GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, m_Position)));

		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, m_Normal)));

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, m_TexCoord)));

		glBindVertexArray(0);
	}

}
}
