#pragma once

#include <glm/common.hpp>

namespace sparky { namespace graphics {

	struct PointLight
	{
		/*glm::vec3 ambient, diffuse, specular;
		glm::vec3 position;
		float constant, linear, quadratic;*/
		/*glm::vec3 ambient;
		float constant;
		glm::vec3 diffuse;
		float linear;
		glm::vec3 specular;
		float quadratic;
		glm::vec3 position;
		float padding;
		PointLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& position, const glm::vec3& attenuation)
			:
			ambient(ambient), diffuse(diffuse), specular(specular), position(position), padding(0.0f)
		{
			constant = attenuation.x;
			linear = attenuation.y;
			quadratic = attenuation.z;
		}

		PointLight(const glm::vec3 lightColor, const glm::vec3& position, const glm::vec3& attenuation)
			:
			ambient(lightColor * 0.2f), diffuse(lightColor * 0.7f), specular(lightColor), position(position), padding(0.0f)
		{
			constant = attenuation.x;
			linear = attenuation.y;
			quadratic = attenuation.z;
		}*/
		glm::vec4 lightColor;
		glm::vec4 position;
		glm::vec4 attenuation;

		PointLight(const glm::vec3 lightColor, const glm::vec3& position, const glm::vec3& attenuation)
			:
			lightColor(glm::vec4(lightColor, 1.0f)), position(glm::vec4(position, 1.0f)), attenuation(glm::vec4(attenuation, 0.0f))
		{
		}
	};

	struct DirectionLight
	{
		glm::vec3 ambient, diffuse, specular;
		glm::vec3 direction;
		DirectionLight(const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, const glm::vec3& direction)
			:
			ambient(ambient), diffuse(diffuse), specular(specular), direction(direction)
		{
		}
	};

}}