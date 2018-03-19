#pragma once

#include "../maths/maths.h"
#include <GL/glew.h>

namespace sparky { namespace graphics {

	class Shader
	{
	private:
		GLuint m_ShaderID;
		const char* m_VertPath;
		const char* m_FragPath;
	public:
		Shader(const char* vertexPath, const char* fragPath);
		~Shader();
		GLuint getShaderID() const { return m_ShaderID; }

		void setUniform1f(const GLchar* name, float value);
		void setUniform1i(const GLchar* name, int value);
		void setUniform2f(const GLchar* name, const glm::vec2& value);
		void setUniform3f(const GLchar* name, const glm::vec3& value);
		void setUniform4f(const GLchar* name, const glm::vec4& value);
		void setUniformMat4(const GLchar* name, const glm::mat4& matrix);

		void enable() const;
		void disable() const;
	private:
		GLuint load();
		GLint getUniformLocation(const GLchar* name);
	};

	}
}