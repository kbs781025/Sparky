#include "label.h"

namespace sparky { namespace graphics {

	Label::Label(const std::string& text, float x, float y, maths::vec4& color)
		:
		Renderable2D(maths::vec3(x, y, 0.0f), maths::vec2(5.0f, 5.0f), color),
		m_Text(text)
	{
	}

	void Label::submit(Renderer2D * renderer) const
	{
		renderer->drawString(m_Text, m_Position.x, m_Position.y, m_Color);
	}
}
}