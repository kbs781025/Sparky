#pragma once
#include <vector>
#include "../maths/maths.h"
#include "Texture2D.h"
#include "shaders.h"
namespace sparky { namespace graphics {

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	struct Vertex;

	class Mesh
	{
	public:
		Mesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies, const std::vector<Texture2D>& textures);
		void Draw(Shader& shader, bool textureOn = true);
		void DrawInstances(Shader& shader, unsigned int instanceCount);

		unsigned int getVAOHandle();
	private:
		VertexArray* m_pVAO;
		std::vector<Vertex> m_Vertices;
		std::vector<unsigned int> m_Indicies;
		std::vector<Texture2D> m_Textures;

		void setupMesh(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indicies);
	};

	

}
}