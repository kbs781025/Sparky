#include "mesh.h"
#include <cstddef>
#include "../platform/opengl/GLCommon.h"

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
		unsigned int diffuseNum = -1;
		unsigned int specularNum = -1;
		unsigned int reflectNum = -1;
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
				else if (type == "texture_reflect")
				{
					number = std::to_string(++reflectNum);
				}

				m_Textures[i].bind(i);
				shader.setUniform1i(("material." + type + number).c_str(), i);
			}
		}
		shader.setUniform1f("material.shininess", 256.0);

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawElements(GL_TRIANGLES, m_Indicies.size(), GL_UNSIGNED_INT, 0));
		GLCall(glBindVertexArray(0));

		GLCall(glActiveTexture(GL_TEXTURE0));
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
			std::string type = m_Textures[i].getType();

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

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glDrawElementsInstanced(GL_TRIANGLES, m_Indicies.size(), GL_UNSIGNED_INT, 0, instanceCount));
		GLCall(glBindVertexArray(0));

		GLCall(glActiveTexture(GL_TEXTURE0));
	}

	void Mesh::setupMesh()
	{
		GLCall(glGenVertexArrays(1, &m_VAO));
		GLCall(glGenBuffers(1, &m_VBO));
		GLCall(glGenBuffers(1, &m_EBO));

		GLCall(glBindVertexArray(m_VAO));
		GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_VBO));
		GLCall(glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), m_Vertices.data(), GL_STATIC_DRAW));
		
		GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO));
		GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indicies.size() * sizeof(unsigned int), m_Indicies.data(), GL_STATIC_DRAW));

		GLCall(glEnableVertexAttribArray(0));
		GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, m_Position))));

		GLCall(glEnableVertexAttribArray(1));
		GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, m_Normal))));

		GLCall(glEnableVertexAttribArray(2));
		GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, m_TexCoord))));

		GLCall(glBindVertexArray(0));
	}

}
}
