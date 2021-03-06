#include <cstddef>
#include "batchrenderer2d.h"
#include "renderable2d.h"

namespace sparky { namespace graphics {
	BatchRenderer2D::BatchRenderer2D()
	{
		init();
	}
	BatchRenderer2D::~BatchRenderer2D()
	{
		delete m_IBO;
		glDeleteBuffers(1, &m_VBO);
		glDeleteBuffers(1, &m_VAO);
	}
	void BatchRenderer2D::begin()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		m_Buffer = (VertexData*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	}
	void BatchRenderer2D::submit(const Renderable2D * renderable)
	{
		const maths::vec3& position = renderable->getPosition();
		const maths::vec2& size= renderable->getSize();
		const maths::vec4& color = renderable->getColor();
		const std::vector<maths::vec2>& uv = renderable->getUV();
		const GLuint textureID = renderable->getTextureID();

		unsigned int c = 0;

		float ts = 0.0f;
		if (textureID > 0) // Texture exists. we don't need to specify the color
		{
			bool found = false;
			
			for (size_t i = 0; i < m_TextureSlots.size(); i++)
			{
				if (m_TextureSlots[i] == textureID)
				{
					found = true;
					ts = (float)i; // use index as actual sampler value
					break;
				}
			}

			if (!found)
			{
				if (m_TextureSlots.size() >= 32)
				{
					end();
					flush();
					begin();
					// texture slot could grow than 32!!
				}

				m_TextureSlots.push_back(textureID);
				ts = (float)(m_TextureSlots.size() - 1);
			}
		}
		else
		{
			int r = (int)(color.x * 255.0f);
			int g = (int)(color.y * 255.0f);
			int b = (int)(color.z * 255.0f);
			int a = (int)(color.w * 255.0f);

			c = a << 24 | b << 16 | g << 8 | r; // ABGR order
		}

		//unsigned int c = r << 24 | g << 16 | b << 8 | a; // inversed channel order

		m_Buffer->vertex = *m_TransformationBack * position;
		m_Buffer->uv = uv[0];
		m_Buffer->textureID = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size.y, position.z);
		m_Buffer->uv = uv[1];
		m_Buffer->textureID = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size.y, position.z);
		m_Buffer->uv = uv[2];
		m_Buffer->textureID = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, position.z);
		m_Buffer->uv = uv[3];
		m_Buffer->textureID = ts;
		m_Buffer->color = c;
		m_Buffer++;

		m_IndexCount += 6;
	}

	void BatchRenderer2D::end()
	{
		glUnmapBuffer(GL_ARRAY_BUFFER);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void BatchRenderer2D::flush()
	{
		for (int i = 0; i < m_TextureSlots.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
		}

		glBindVertexArray(m_VAO);
		m_IBO->bind();

		glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, nullptr); // One Draw Call for all sprites

		m_IBO->unbind();
		glBindVertexArray(0);

		m_IndexCount = 0;
	}

	void BatchRenderer2D::init()
	{
		glGenVertexArrays(1, &m_VAO);

		glGenBuffers(1, &m_VBO);

		glBindVertexArray(m_VAO);

		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, nullptr, GL_DYNAMIC_DRAW); // not a static but dynamic draw

		glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
		glEnableVertexAttribArray(SHADER_UV_INDEX);
		glEnableVertexAttribArray(SHADER_TID_INDEX);
		glEnableVertexAttribArray(SHADER_COLOR_INDEX);

		glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
		glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::uv)));
		glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::textureID)));
		glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*) (offsetof(VertexData, VertexData::color)));

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GLushort indices[RENDERER_INDICES_SIZE];

		int offset = 0;
		for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
		{
			indices[  i  ] = offset + 0;
			indices[i + 1] = offset + 1;
			indices[i + 2] = offset + 2;

			indices[i + 3] = offset + 2;
			indices[i + 4] = offset + 3;
			indices[i + 5] = offset + 0;

			offset += 4;
		}

		m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

		glBindVertexArray(0);
	}

}
}