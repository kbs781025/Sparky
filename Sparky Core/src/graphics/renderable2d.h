#pragma once

#include <GL/glew.h>

#include "buffers/buffer.h"
#include "buffers/indexbuffer.h"
#include "buffers/vertexarray.h"

#include "../maths/maths.h"
#include "shaders.h"
#include "renderer2d.h"
#include "texture.h"

namespace sparky { namespace graphics {

	struct VertexData
	{
		maths::vec3 vertex;
		maths::vec2 uv;
		GLuint color;
	};

	class Renderable2D
	{
	protected:
		maths::vec3 m_Position;
		maths::vec2 m_Size;
		maths::vec4 m_Color;
		std::vector<maths::vec2> m_UV;
		Texture* m_Texture;
	private:
		void setDefaultUV()
		{
			m_UV.resize(4);

			m_UV[0].x = 0.0f;
			m_UV[0].y = 0.0f;

			m_UV[1].x = 0.0f;
			m_UV[1].y = 1.0f;

			m_UV[2].x = 1.0f;
			m_UV[2].y = 1.0f;

			m_UV[3].x = 1.0f;
			m_UV[3].y = 0.0f;
		}
	protected:
		Renderable2D() {}
	public:
		Renderable2D(maths::vec3 position, maths::vec2 size, maths::vec4 color, Texture* texture = nullptr)
			:
			m_Position(position), m_Size(size), m_Color(color), m_Texture(texture)
		{
			setDefaultUV();
		}

		virtual ~Renderable2D()
		{}

		virtual void submit(Renderer2D* renderer) const
		{
			renderer->submit(this);
		}

		inline const maths::vec3& getPosition() const { return m_Position; }
		inline const maths::vec2& getSize() const { return m_Size; }
		inline const maths::vec4& getColor() const { return m_Color; }
		inline const std::vector<maths::vec2>& getUV() const { return m_UV; }
	};

	}
}