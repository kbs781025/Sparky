#include "shaders.h"
#include "../utils/FileUtils.h"

#include <vector>
#include <iostream>

namespace sparky { namespace graphics {

	Shader::Shader(const char * vertexPath, const char * fragPath, const char * geoPath)
		:
		m_VertPath(vertexPath),
		m_FragPath(fragPath),
		m_GeoPath(geoPath),
		m_Enabled(false)
	{
		loadShaders();
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

	void Shader::loadShaders()
	{
		m_ShaderID = glCreateProgram();
		GLuint vertex = compileShader(GL_VERTEX_SHADER, m_VertPath);
		GLuint fragment = compileShader(GL_FRAGMENT_SHADER, m_FragPath);
		GLuint geoMetry = 0;
		if (m_GeoPath)
		{
			geoMetry = compileShader(GL_GEOMETRY_SHADER, m_GeoPath);
		}

		linkShader(vertex, fragment, geoMetry);
	}

	GLuint Shader::compileShader(GLuint shaderType, const char* filePath)
	{
		GLuint shaderID = glCreateShader(shaderType);

		std::string sourceString = read_file(filePath);
		const char* shaderSource = sourceString.c_str();
		glShaderSource(shaderID, 1, &shaderSource, nullptr);
		glCompileShader(shaderID);

		GLint result;
		glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result);

		if (result == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength);
			std::vector<GLchar> error(maxLength);
			glGetShaderInfoLog(shaderID, maxLength, &maxLength, error.data());
			std::cout << "Failed to compile shader. " << error.data() << std::endl;
			glDeleteShader(shaderID);

			return 0;
		}

		return shaderID;
	}

	void Shader::linkShader(GLuint vertShader, GLuint fragShader, GLuint geoShader)
	{
		glAttachShader(m_ShaderID, vertShader);
		glAttachShader(m_ShaderID, fragShader);
		if (geoShader > 0)
		{
			glAttachShader(m_ShaderID, geoShader);
		}

		glLinkProgram(m_ShaderID);

		GLint isLinked = 0;
		glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength);

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]);
			std::cout << "Failed to compiled shader : " << infoLog.data() << std::endl;

			// The program is useless now. So delete it.
			glDeleteProgram(m_ShaderID);

			// Provide the infolog in whatever manner you deem best.
			// Exit with failure.
			m_ShaderID = 0;
			return;
		}

		glValidateProgram(m_ShaderID);

		glDeleteShader(vertShader);
		glDeleteShader(fragShader);
		glDeleteShader(geoShader);
	}

	GLint Shader::getUniformLocation(const GLchar * name)
	{
		return glGetUniformLocation(m_ShaderID, name);
	}

}
}