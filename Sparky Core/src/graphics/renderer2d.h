#pragma once

#include <GL/glew.h>	
#include "../maths/maths.h"
#include <vector>


namespace sparky { namespace graphics {
	
	class Renderable2D;

	class Renderer2D
	{
	protected:
		std::vector<maths::mat4> m_TransformationStack;
		const maths::mat4* m_TransformationBack;
	protected:
		Renderer2D()
		{
			m_TransformationStack.push_back(maths::mat4::identity());
			m_TransformationBack = &m_TransformationStack.back();
		}
	public:
		void push(const maths::mat4& matrix, bool override = false)
		{
			if (override == false)
			{
				m_TransformationStack.push_back(matrix);
			}
			else
			{
				m_TransformationStack.push_back(m_TransformationStack.back() * matrix);
			}

			m_TransformationBack = &m_TransformationStack.back();
		}

		void pop()
		{
			if (m_TransformationStack.size() > 1)
			{
				m_TransformationStack.pop_back();
			}

			m_TransformationBack = &m_TransformationStack.back();
		}

		virtual void begin() {}
		virtual void submit(const Renderable2D* renderable) = 0;
		virtual void drawString(const std::string& text, float x, float y, const maths::vec4& color) = 0;
		virtual void end() {}
		virtual void flush() = 0;
	};

	}
}