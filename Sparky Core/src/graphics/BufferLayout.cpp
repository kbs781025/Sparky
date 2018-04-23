#include "BufferLayout.h"

sparky::graphics::BufferLayout::BufferLayout()
	: m_Size(0)
{
}

sparky::graphics::BufferLayout::BufferLayout(std::vector<BufferElement>& elements)
	: m_Elements(elements)
{
}

void sparky::graphics::BufferLayout::Push(const std::string & name, GLuint count, GLenum type, bool normalize)
{
	m_Elements.emplace_back(name, count, type, normalize);
	m_Size += count * sizeof(type);
}

sparky::graphics::BufferLayout sparky::graphics::BufferLayout::getPosNormTexLayout()
{
	BufferLayout bufferlayout;
	bufferlayout.Push<glm::vec3>("POSITION", false);
	bufferlayout.Push<glm::vec3>("NORMAL", false);
	bufferlayout.Push<glm::vec2>("TEXCOORD", false);

	return bufferlayout;
}

sparky::graphics::BufferLayout sparky::graphics::BufferLayout::getPosTexLayout()
{
	BufferLayout bufferlayout;
	bufferlayout.Push<glm::vec3>("POSITION", false);
	bufferlayout.Push<glm::vec2>("TEXCOORD", false);

	return bufferlayout;
}
