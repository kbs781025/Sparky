#include "shaders.h"
#include "../utils/FileUtils.h"
#include "Light.h"
#include "UniformBuffer.h"
#include "../platform/opengl/GLCommon.h"

#include <vector>
#include <iostream>

namespace sparky { namespace graphics {

	const std::string Shader::POINTLIGHT = "pointLights";
	const std::string Shader::DIRECTIONLIGHT = "dirLights";
	const std::string Shader::MATERIAL = "material";

	Shader::Shader(const char * vertexPath, const char * fragPath, const char * geoPath)
		:
		m_VertPath(vertexPath),
		m_FragPath(fragPath),
		m_GeoPath(geoPath)
	{
		loadShaders();
	}

	Shader::~Shader()
	{
		GLCall(glDeleteProgram(m_ShaderID));
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
		GLCall(unsigned int uniformBlockIndex = glGetUniformBlockIndex(m_ShaderID, name));

		#ifdef _DEBUG
		if (uniformBlockIndex < 0)
		{
			std::cout << "Unable to find uniform block : " << name << std::endl;
			__debugbreak();
			return;
		}
		#endif

		GLCall(glUniformBlockBinding(m_ShaderID, uniformBlockIndex, bindingPoint));
	}

	GLuint Shader::getBlockBindingPoint(const GLchar * name) const
	{
		GLCall(return glGetUniformBlockIndex(m_ShaderID, name));
	}

	void Shader::enable() const
	{
		GLCall(glUseProgram(m_ShaderID));
	}

	void Shader::disable() const
	{
		GLCall(glUseProgram(0));
	}

	void Shader::loadShaders()
	{
		GLCall(m_ShaderID = glCreateProgram());
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
		GLCall(GLuint shaderID = glCreateShader(shaderType));

		std::string sourceString = read_file(filePath);
		if (sourceString == "")
			return 0;

		const char* shaderSource = sourceString.c_str();
		GLCall(glShaderSource(shaderID, 1, &shaderSource, nullptr));;
		GLCall(glCompileShader(shaderID));;

		GLint result;
		GLCall(glGetShaderiv(shaderID, GL_COMPILE_STATUS, &result));

		if (result == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetShaderiv(shaderID, GL_INFO_LOG_LENGTH, &maxLength));
			std::vector<GLchar> error(maxLength);
			GLCall(glGetShaderInfoLog(shaderID, maxLength, &maxLength, error.data()));
			std::cout << filePath << std::endl;
			std::cout << "Failed to compile shader. " << error.data() << std::endl;
			GLCall(glDeleteShader(shaderID));

			return 0;
		}

		return shaderID;
	}

	void Shader::linkShader(GLuint vertShader, GLuint fragShader, GLuint geoShader)
	{
		GLCall(glAttachShader(m_ShaderID, vertShader));
		GLCall(glAttachShader(m_ShaderID, fragShader));
		if (geoShader > 0)
		{
			GLCall(glAttachShader(m_ShaderID, geoShader));
		}

		GLCall(glLinkProgram(m_ShaderID));

		GLint isLinked = 0;
		GLCall(glGetProgramiv(m_ShaderID, GL_LINK_STATUS, &isLinked));
		if (isLinked == GL_FALSE)
		{
			GLint maxLength = 0;
			GLCall(glGetProgramiv(m_ShaderID, GL_INFO_LOG_LENGTH, &maxLength));

			// The maxLength includes the NULL character
			std::vector<GLchar> infoLog(maxLength);
			GLCall(glGetProgramInfoLog(m_ShaderID, maxLength, &maxLength, &infoLog[0]));
			std::cout << "Failed to compiled shader : " << infoLog.data() << std::endl;

			// The program is useless now. So delete it.
			GLCall(glDeleteProgram(m_ShaderID));

			// Provide the infolog in whatever manner you deem best.
			// Exit with failure.
			m_ShaderID = 0;
			return;
		}

		GLCall(glValidateProgram(m_ShaderID));

		GLCall(glDeleteShader(vertShader));
		GLCall(glDeleteShader(fragShader));
		GLCall(glDeleteShader(geoShader));
	}

	GLint Shader::getUniformLocation(const GLchar * name) const
	{
		GLCall(return glGetUniformLocation(m_ShaderID, name));
	}

}
}