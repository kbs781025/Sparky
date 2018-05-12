#pragma once

#include "../maths/maths.h"
#include <GL/glew.h>
#include <vector>
#include <string>

namespace sparky { namespace graphics {
	
	struct PointLight;
	struct DirectionLight;
	class UniformBuffer;

	class Shader
	{
	private:
		GLuint m_ShaderID;
		const char* m_VertPath;
		const char* m_FragPath;
		const char* m_GeoPath;
	public:
		Shader(const char* vertexPath, const char* fragPath, const char* geoPath = nullptr);
		~Shader();

		void setUniform1f(const GLchar* name, float value) const;
		void setUniform1i(const GLchar* name, int value) const;
		void setUniform2f(const GLchar* name, const glm::vec2& value) const;
		void setUniform3f(const GLchar* name, const glm::vec3& value) const;
		void setUniform4f(const GLchar* name, const glm::vec4& value) const;
		void setUniformMat4(const GLchar* name, const glm::mat4& matrix) const;
		void bindUniformBlock(const GLchar* name, GLuint bindingPoint) const;

		GLuint getShaderID() const { return m_ShaderID; }
		GLuint getBlockBindingPoint(const GLchar* name) const;

		void enable() const;
		void disable() const;
	private:
		void loadShaders();
		GLuint compileShader(GLuint shaderType, const char* filePath);
		void linkShader(GLuint vertShader, GLuint fragShader, GLuint geoShader = 0);

		GLint getUniformLocation(const GLchar* name) const;
	private:
		static const std::string POINTLIGHT;
		static const std::string DIRECTIONLIGHT;
		static const std::string MATERIAL;
	};

}
}