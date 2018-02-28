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
		float textureID;
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
	protected:
		Renderable2D() 
		{
			setUVDefault();
		}
	public:
		Renderable2D(maths::vec3 position, maths::vec2 size, maths::vec4 color)
			:
			m_Position(position), m_Size(size), m_Color(color)
		{
			setUVDefault();
		}

		virtual ~Renderable2D()
		{
			delete m_Texture;
		}

		virtual void submit(Renderer2D* renderer) const
		{
			renderer->submit(this);
		}

		inline const maths::vec3& getPosition() const { return m_Position; }
		inline const maths::vec2& getSize() const { return m_Size; }
		inline const maths::vec4& getColor() const { return m_Color; }
		inline const std::vector<maths::vec2>& getUV() const { return m_UV; }

		inline const GLuint getTextureID() const { return m_Texture == nullptr ? 0 : m_Texture->getTextureID(); }
	private:
		void setUVDefault()
		{
			m_UV.emplace_back(0.0f, 0.0f);
			m_UV.emplace_back(0.0f, 1.0f);
			m_UV.emplace_back(1.0f, 1.0f);
			m_UV.emplace_back(1.0f, 0.0f);
		}
	};

	}
}