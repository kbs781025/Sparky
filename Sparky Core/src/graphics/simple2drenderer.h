#pragma once

#include <deque> // double ended queue
#include "renderer2d.h"
#include "staticsprite.h"

namespace sparky { namespace graphics {

	class Simple2DRenderer : public Renderer2D
	{
	private:
		std::deque<const StaticSprite*> m_RenderQueue;
	public:
		virtual void submit(const Renderable2D* renderable) override;
		virtual void flush() override;
	};


	}
}