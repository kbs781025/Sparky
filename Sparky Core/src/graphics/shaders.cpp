#include "shaders.h"
#include "../utils/FileUtils.h"
#include "Light.h"
#include "UniformBuffer.h"
#include "../platform/opengl/GLCommon.h"

#include <vector>
#include <iostream>

namespace sparky { namespace graphics {

	Shader::Shader(GLuint* shaderID)
		:
		m_pShaderID(shaderID)
	{
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(*m_pShaderID));
	}

	void Shader::setUniform1f(const GLchar * name, float value) const
	{
		GLCall(GLint uniformLocation = getUniformLocation(name));

		#ifdef _DEBUG
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniform1f(uniformLocation, value));
	}

	void Shader::setUniform1i(const GLchar * name, int value) const
	{
		GLCall(GLint uniformLocation = getUniformLocation(name));

		#ifdef _DEBUG
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniform1i(uniformLocation, value));
	}

	void Shader::setUniform2f(const GLchar * name, const glm::vec2 & value) const
	{
		GLCall(GLint uniformLocation = getUniformLocation(name));

		#ifdef _DEBUG
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniform2f(uniformLocation, value.x, value.y));
	}

	void Shader::setUniform3f(const GLchar * name, const glm::vec3 & value) const
	{
		GLCall(GLint uniformLocation = getUniformLocation(name));

		#ifdef _DEBUG
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniform3f(uniformLocation, value.x, value.y, value.z));
	}

	void Shader::setUniform4f(const GLchar * name, const glm::vec4 & value) const
	{
		GLCall(GLint uniformLocation = getUniformLocation(name));

		#ifdef _DEBUG
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniform4f(uniformLocation, value.x, value.y, value.z, value.w));
	}

	void Shader::setUniformMat4(const GLchar * name, const glm::mat4 & matrix) const
	{
		GLCall(GLint uniformLocation = getUniformLocation(name));

		#ifdef _DEBUG
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix)));
	}

	void Shader::bindUniformBlock(const GLchar * name, GLuint bindingPoint) const
	{
		GLCall(unsigned int uniformBlockIndex = glGetUniformBlockIndex(*m_pShaderID, name));

		#ifdef _DEBUG
		if (uniformBlockIndex < 0)
		{
			std::cout << "Unable to find uniform block : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniformBlockBinding(*m_pShaderID, uniformBlockIndex, bindingPoint));
	}

	GLuint Shader::getBlockBindingPoint(const GLchar * name) const
	{
		GLCall(return glGetUniformBlockIndex(*m_pShaderID, name));
	}

	void Shader::enable() const
	{
		GLCall(glUseProgram(*m_pShaderID));
	}

	void Shader::disable() const
	{
		GLCall(glUseProgram(0));
	}

	GLint Shader::getUniformLocation(const GLchar * name) const
	{
		GLCall(return glGetUniformLocation(*m_pShaderID, name));
	}

}
}