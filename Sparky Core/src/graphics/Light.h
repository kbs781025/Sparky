#pragma once

#include <glm/common.hpp>

namespace sparky { namespace graphics {

	struct Light 
	{
		// TODO : position, direction, attenuation sholud be vec3
		glm::vec4 lightColor;
		glm::vec4 position;
		glm::vec4 direction;
		glm::vec4 attenuation;

		Light() = default;
		Light(const glm::vec4 lightColor, const glm::vec3& position, const glm::vec3& direction, const glm::vec3& attenuation)
			:
			lightColor(lightColor), 
			position(glm::vec4(position, 1.0f)), 
			direction(glm::vec4(direction, 1.0f)),
			attenuation(glm::vec4(attenuation, 0.0f))
		{
		}
	};

}}