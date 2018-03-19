#pragma once

#include <vector>
#include "../renderable2d.h"
#include "../renderer2d.h"

namespace sparky { namespace graphics {

	class Group : public Renderable2D
	{
	private:
		std::vector<Renderable2D*> m_Renderables;
		glm::mat4 m_TransforamtionMatirx;
	public:
		Group(const glm::mat4& transform);
		void submit(Renderer2D* renderer) const override;
		void add(Renderable2D* renderable);
	};

	}
}