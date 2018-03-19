
#include "group.h"

namespace sparky { namespace graphics {
	
	Group::Group(const glm::mat4& transform)
		:
		m_TransforamtionMatirx(transform)
	{
	}

	void Group::submit(Renderer2D * renderer) const
	{
		renderer->push(m_TransforamtionMatirx);

		for (const auto renderable : m_Renderables)
		{
			renderable->submit(renderer);
		}

		renderer->pop();
	}

	void Group::add(Renderable2D * renderable)
	{
		m_Renderables.push_back(renderable);
	}

}
}