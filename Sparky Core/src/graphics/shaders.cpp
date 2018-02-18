#include "shaders.h"
#include "../utils/FileUtils.h"

#include <vector>
#include <iostream>

namespace sparky { namespace graphics {

	Shader::Shader(const char * vertexPath, const char * fragPath)
		:
		m_VertPath(vertexPath),
		m_FragPath(fragPath)
	{
		m_ShaderID = load();
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_ShaderID);
	}

	void Shader::setUniform1f(const GLchar * name, float value)
	{
		glUniform1f(getUniformLocation(name), value);
	}

	void Shader::setUniform1i(const GLchar * name, int value)
	{
		glUniform1i(getUniformLocation(name), value);
	}

	void Shader::setUniform2f(const GLchar * name, const maths::vec2 & value)
	{
		glUniform2f(getUniformLocation(name), value.x, value.y);
	}

	void Shader::setUniform3f(const GLchar * name, const maths::vec3 & value)
	{
		glUniform3f(getUniformLocation(name), value.x, value.y, value.z);
	}

	void Shader::setUniform4f(const GLchar * name, const maths::vec4 & value)
	{
		glUniform4f(getUniformLocation(name), value.x, value.y, value.z, value.w);
	}

	void Shader::setUniformMat4(const GLchar * name, const maths::mat4 & matrix)
	{
		glUniformMatrix4fv(getUniformLocation(name), 1, GL_FALSE, matrix.elements);
	}

	void Shader::enable() const
	{
		glUseProgram(m_ShaderID);
	}

	void Shader::disable() const
	{
		glUseProgram(0);
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