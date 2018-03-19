#include "layer.h"
namespace sparky { namespace graphics {
	Layer::Layer(Renderer2D * renderer, Shader * shader, glm::mat4 projectionMatrix)
		:
		m_Renderer(renderer),
		m_Shader(shader),
		m_ProjectionMatrix(projectionMatrix)
	{
		shader->enable();
		shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);
		shader->disable();
	}

	Layer::Layer()
	{
	}

	Layer::~Layer()
	{
		delete m_Shader;
		delete m_Renderer;

		for (auto& renderable : m_Renderables)
		{
			delete renderable;
		}
	}

	void Layer::add(Renderable2D* renderable)
	{
		m_Renderables.push_back(renderable);
	}

	void Layer::render()
	{
		m_Shader->enable();

		m_Renderer->begin();

		for (const auto renderable : m_Renderables)
		{
			renderable->submit(m_Renderer);
		}

		m_Renderer->end();

		m_Renderer->flush();
	}
}
}
