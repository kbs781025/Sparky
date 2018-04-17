#pragma once
#include <vector>
#include "../maths/maths.h"
#include "texture.h"
#include "shaders.h"
namespace sparky { namespace graphics {

	struct Vertex
	{
		glm::vec3 m_Position;
		glm::vec3 m_Normal;
		glm::vec2 m_TexCoord;
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indicies, std::vector<Texture> textures);
		void Draw(Shader& shader, bool textureOn = true);
		void DrawInstances(Shader& shader, unsigned int instanceCount);

		unsigned int getVAO() { return m_VAO; }
	private:
		unsigned int m_VAO, m_VBO, m_EBO;
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indicies;
		std::vector<Texture> m_Textures;

		void setupMesh();
	};

	

}
}