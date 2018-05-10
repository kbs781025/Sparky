#pragma once

namespace sparky { namespace graphics {

	struct Vertex
	{
		glm::vec3 m_Position;
		glm::vec2 m_TexCoord;
		glm::vec3 m_Tangent;
		glm::vec3 m_BiTangent;
		glm::vec3 m_Normal;
	};

}}