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

void sparky::graphics::BufferLayout::PushPosition()
{
	Push<glm::vec3>("POSITION", false);
}

void sparky::graphics::BufferLayout::PushNormal()
{
	Push<glm::vec3>("NORMAL", false);
}

void sparky::graphics::BufferLayout::PushTexCoord()
{
	Push<glm::vec2>("TEXCOORD", false);
}

sparky::graphics::BufferLayout sparky::graphics::BufferLayout::getPosNormTexLayout()
{
	BufferLayout bufferlayout;
	bufferlayout.PushPosition();
	bufferlayout.PushNormal();
	bufferlayout.PushTexCoord();

	return bufferlayout;
}

sparky::graphics::BufferLayout sparky::graphics::BufferLayout::getPosTexLayout()
{
	BufferLayout bufferlayout;
	bufferlayout.PushPosition();
	bufferlayout.PushTexCoord();

	return bufferlayout;
}

sparky::graphics::BufferLayout sparky::graphics::BufferLayout::getPosLayout()
{
	BufferLayout bufferLayout;
	bufferLayout.PushPosition();

	return bufferLayout;
}
