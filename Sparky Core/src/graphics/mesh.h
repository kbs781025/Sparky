#pragma once

#include <vector>

#include "../maths/maths.h"
#include "Texture2D.h"
#include "Material.h"

namespace sparky { namespace graphics {

	class VertexArray;
	class VertexBuffer;
	class IndexBuffer;
	class BufferLayout;
	class Renderer3D;
	class Shader;

	class Mesh
	{
	public:
		Mesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies, Material* material);
		void Draw() const;
		void DrawInstances(unsigned int instanceCount) const;
		void Render(const Renderer3D& renderer) const;

		unsigned int getVAOHandle();
	private:
		VertexArray* m_pVAO;
		Material* m_Material;

		void setupMesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies);
	};

	

}
}