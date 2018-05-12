#pragma once
#include <vector>
#include "../maths/maths.h"
#include "Texture2D.h"
#include "shaders.h"
namespace sparky { namespace graphics {

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class BufferLayout;

	class Mesh
	{
	public:
		Mesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies, const std::vector<Texture2D>& textures);
		void Draw(Shader& shader, bool textureOn = true);
		void DrawInstances(Shader& shader, unsigned int instanceCount);

		unsigned int getVAOHandle();
	private:
		VertexArray* m_pVAO;
		std::vector<Texture2D> m_Textures;

		void setupMesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies);
	};

	

}
}