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
	class Renderer3D;

	class Mesh
	{
	public:
		Mesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies, const std::vector<Texture2D>& textures);
		void Draw(const Shader& shader, bool textureOn = true) const;
		void DrawInstances(const Shader& shader, unsigned int instanceCount) const;
		void Render(const Renderer3D& renderer) const;

		unsigned int getVAOHandle();
	private:
		VertexArray* m_pVAO;
		std::vector<Texture2D> m_Textures;

		void setupMesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies);
	};

	

}
}