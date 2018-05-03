#pragma once

#include <glm/common.hpp>

namespace sparky { namespace graphics {

	enum class LightUniform
	{
		
	};

	struct PointLight
	{
		glm::vec4 ambient, diffuse, specular;
		glm::vec3 position;
		float constant, linear, quadratic;
		PointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position, const glm::vec3& attenuation)
			:
			ambient(ambient), diffuse(diffuse), specular(specular), position(position)
		{
			constant = attenuation.x;
			diffuse = attenuation.y;
			specular = attenuation.z;
		}
	};

	struct DirectionLight
	{
		glm::vec4 ambient, diffuse, specular;
		glm::vec3 direction;
		DirectionLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction)
			:
			ambient(ambient), diffuse(diffuse), specular(specular), direction(direction)
		{
		}
	};

}}