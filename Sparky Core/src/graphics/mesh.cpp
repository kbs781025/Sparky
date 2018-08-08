#include <cstddef>

#include "../../res/shaders/preamble.glsl"

#include "../platform/opengl/GLCommon.h"

#include "mesh.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "Texture2D.h"
#include "shaders.h"

namespace sparky { namespace graphics {

	Mesh::Mesh(const std::vector<float>& vertices, const BufferLayout& layout, const std::vector<unsigned int>& indicies, Material* material)
		:
		m_pVAO(nullptr),
		m_Material(material)
	{
		setupMesh(vertices, layout, indicies);
	}

	void Mesh::Draw() const
	{
		m_Material->GetShader()->enable();
		m_Material->Bind();
		
		m_Material->GetShader()->setUniform1f(SPECULAR_SHININESS_LOCATION, 128.0);

		m_pVAO->Draw();
	}

	void Mesh::DrawInstances(unsigned int instanceCount) const
	{
		m_Material->GetShader()->enable();
		m_Material->Bind();

		m_Material->GetShader()->setUniform1f(SPECULAR_SHININESS_LOCATION, 128.0);

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
