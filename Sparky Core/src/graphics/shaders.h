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
		bool m_Enabled;
	public:
		Shader(const char* vertexPath, const char* fragPath, const char* geoPath = nullptr);
		~Shader();

		void setUniform1f(const GLchar* name, float value);
		void setUniform1i(const GLchar* name, int value);
		void setUniform2f(const GLchar* name, const glm::vec2& value);
		void setUniform3f(const GLchar* name, const glm::vec3& value);
		void setUniform4f(const GLchar* name, const glm::vec4& value);
		void setUniformMat4(const GLchar* name, const glm::mat4& matrix);
		void bindUniformBlock(const GLchar* name, GLuint bindingPoint);

		

		GLuint getShaderID() const { return m_ShaderID; }
		GLuint getBlockBindingPoint(const GLchar* name);

		void setPointLight(const std::vector<PointLight>& pointLight);
		void setDirLight(const std::vector<DirectionLight>& dirLight);

		void enable();
		void disable();
	private:
		void loadShaders();
		GLuint compileShader(GLuint shaderType, const char* filePath);
		void linkShader(GLuint vertShader, GLuint fragShader, GLuint geoShader = 0);

		GLint getUniformLocation(const GLchar* name);
	private:
		static const std::string POINTLIGHT;
		static const std::string DIRECTIONLIGHT;
		static const std::string MATERIAL;
	};

}
}