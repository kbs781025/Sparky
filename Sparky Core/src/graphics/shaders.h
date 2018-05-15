#pragma once

#include <vector>
#include <string>
#include <GL/glew.h>

#include "../maths/maths.h"

namespace sparky { namespace graphics {
	
	struct PointLight;
	struct DirectionLight;
	class UniformBuffer;

	class Shader
	{
	private:
		GLuint* m_pShaderID;
	public:
		Shader(GLuint* shaderID);
		~Shader();

		void setUniform1f(const GLchar* name, float value) const;
		void setUniform1i(const GLchar* name, int value) const;
		void setUniform2f(const GLchar* name, const glm::vec2& value) const;
		void setUniform3f(const GLchar* name, const glm::vec3& value) const;
		void setUniform4f(const GLchar* name, const glm::vec4& value) const;
		void setUniformMat4(const GLchar* name, const glm::mat4& matrix) const;
		void bindUniformBlock(const GLchar* name, GLuint bindingPoint) const;

		GLuint getShaderID() const { return *m_pShaderID; }
		GLuint getBlockBindingPoint(const GLchar* name) const;

		void enable() const;
		void disable() const;
	private:
		GLint getUniformLocation(const GLchar* name) const;
	};

}
}