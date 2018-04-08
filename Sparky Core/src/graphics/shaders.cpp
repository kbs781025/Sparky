#include "shaders.h"
#include "../utils/FileUtils.h"

#include <vector>
#include <iostream>

namespace sparky { namespace graphics {

	Shader::Shader(const char * vertexPath, const char * fragPath)
		:
		m_VertPath(vertexPath),
		m_FragPath(fragPath),
		m_Enabled(false)
	{
		m_ShaderID = load();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void Shader::setUniform1f(const GLchar * name, float value)
	{
		if(!m_Enabled)
		{
			std::cout << "Shader is not enabled." << std::endl;
			return;
		}

		GLint uniformLocation = getUniformLocation(name);
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			return;
		}

		glUniform1f(uniformLocation, value);
	}

	void Shader::setUniform1i(const GLchar * name, int value)
	{
		if (!m_Enabled)
		{
			std::cout << "Shader is not enabled." << std::endl;
			return;
		}

		GLint uniformLocation = getUniformLocation(name);
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			return;
		}
		glUniform1i(uniformLocation, value);
	}

	void Shader::setUniform2f(const GLchar * name, const glm::vec2 & value)
	{
		if (!m_Enabled)
		{
			std::cout << "Shader is not enabled." << std::endl;
			return;
		}

		GLint uniformLocation = getUniformLocation(name);
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			return;
		}
		glUniform2f(uniformLocation, value.x, value.y);
	}

	void Shader::setUniform3f(const GLchar * name, const glm::vec3 & value)
	{
		if (!m_Enabled)
		{
			std::cout << "Shader is not enabled." << std::endl;
			return;
		}

		GLint uniformLocation = getUniformLocation(name);
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			return;
		}
		glUniform3f(uniformLocation, value.x, value.y, value.z);
	}

	void Shader::setUniform4f(const GLchar * name, const glm::vec4 & value)
	{
		if (!m_Enabled)
		{
			std::cout << "Shader is not enabled." << std::endl;
			return;
		}

		GLint uniformLocation = getUniformLocation(name);
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			return;
		}
		glUniform4f(uniformLocation, value.x, value.y, value.z, value.w);
	}

	void Shader::setUniformMat4(const GLchar * name, const glm::mat4 & matrix)
	{
		if (!m_Enabled)
		{
			std::cout << "Shader is not enabled." << std::endl;
			return;
		}

		GLint uniformLocation = getUniformLocation(name);
		if (uniformLocation < 0)
		{
			std::cout << "Unable to find uniform : " << name << std::endl;
			return;
		}
		glUniformMatrix4fv(uniformLocation, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::bindUniformBlock(const GLchar * name, GLuint bindingPoint)
	{
		unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_ShaderID, name);
		if (uniformBlockIndex < 0)
		{
			std::cout << "Unable to find uniform block : " << name << std::endl;
			return;
		}
		glUniformBlockBinding(m_ShaderID, uniformBlockIndex, bindingPoint);
	}

	void Shader::enable()
	{
		glUseProgram(m_ShaderID);
		m_Enabled = true;
	}

	void Shader::disable()
	{
		glUseProgram(0);
		m_Enabled = false;
	}

	GLuint Shader::load()
	{
		GLuint program = glCreateProgram();
		GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
		GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);

		std::string vertSourceString = read_file(m_VertPath);
		std::string fragSourceString = read_file(m_FragPath);

		const char* vertSource = vertSourceString.c_str();
		const char* fragSource = fragSourceString.c_str();

		glShaderSource(vertex, 1, &vertSource, nullptr);
		glCompileShader(vertex);

		GLint result;
		glGetShaderiv(vertex, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(vertex, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(vertex, length, &length, error.data());
			std::cout << "Failed to compile vertex shader. " << error.data() << std::endl;
			glDeleteShader(vertex);

			return 0;
		}

		glShaderSource(fragment, 1, &fragSource, nullptr);
		glCompileShader(fragment);

		glGetShaderiv(fragment, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLint length;
			glGetShaderiv(fragment, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> error(length);
			glGetShaderInfoLog(fragment, length, &length, error.data());
			std::cout << "Failed to compile fragment shader. " << error.data() << std::endl;
			glDeleteShader(fragment);

			return 0;
		}

		glAttachShader(program, vertex);
		glAttachShader(program, fragment);

		glLinkProgram(program);

		GLint isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);
			std::cout << "Failed to compiled shader : " << infoLog.data() << std::endl;

			// The program is useless now. So delete it.
			glDeleteProgram(program);

			// Provide the infolog in whatever manner you deem best.
			// Exit with failure.
			return 0;
		}

		glValidateProgram(program);
		
		glDeleteShader(vertex);
		glDeleteShader(fragment);

		return program;
	}

	GLint Shader::getUniformLocation(const GLchar * name)
	{
		return glGetUniformLocation(m_ShaderID, name);
	}

}
}