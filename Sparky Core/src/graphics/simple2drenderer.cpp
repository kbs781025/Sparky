#include "simple2drenderer.h"
#include "renderable2d.h"

namespace sparky { namespace graphics {

	void Simple2DRenderer::submit(const Renderable2D* renderable)
	{
		m_RenderQueue.push_back((const StaticSprite*) renderable);
	}
	void Simple2DRenderer::flush()
	{
		while (m_RenderQueue.empty() == false)
		{
			const StaticSprite* renderable = m_RenderQueue.front();
			renderable->getVAO()->bind();
			renderable->getIBO()->bind();

			renderable->getShader().setUniformMat4("ml_matrix", maths::mat4::translate(maths::vec3(renderable->getPosition())));
			glDrawElements(GL_TRIANGLES, renderable->getIBO()->getCount(), GL_UNSIGNED_SHORT, 0); // One Draw call per vertex buffer

			renderable->getIBO()->unbind();
			renderable->getVAO()->unbind();

			m_RenderQueue.pop_front();
		}
	}

}
}