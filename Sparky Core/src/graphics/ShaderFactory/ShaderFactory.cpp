#include "ShaderFactory.h"
#include <string>
namespace sparky { namespace graphics { namespace ShaderFactory {

	const std::string SHADER_DIR = "src/graphics/ShaderFactory/Shaders/";
	const std::string VERTEX_SHADER = ".vert";
	const std::string FRAGMENT_SHADER = ".frag";
	const std::string GEOMETRY_SHADER = ".geom";
		
	const std::string BasicLight = "basicLightShader";
	const std::string Debug = "debugShader";
	const std::string Normal = "normalVecShader";
	const std::string CreateShadow = "createShadowShader";
	const std::string ApplyShadow = "applyShadowShader";
	const std::string Model = "modelShader";

	Shader * ShaderFactory::BasicLightShader()
	{
		return CreateShader(BasicLight);
	}

	Shader * ShaderFactory::NormalShader()
	{
		return CreateShader(Normal);
	}

	Shader * ShaderFactory::DebugShader()
	{
		return CreateShader(Debug);
	}

	Shader * ShaderFactory::CreateShadowShader()
	{
		return CreateShader(CreateShadow);
	}

	Shader * ShaderFactory::ApplyShadowShader()
	{
		return CreateShader(ApplyShadow);
	}

	Shader * ModelShader()
	{
		return CreateShader(Model);
	}

	Shader * ShaderFactory::CreateShader(const std::string & shaderName)
	{
		std::string vertPathStr = (SHADER_DIR + shaderName + VERTEX_SHADER);
		std::string fragPathStr = (SHADER_DIR + shaderName + FRAGMENT_SHADER);
		std::string geomPathStr = (SHADER_DIR + shaderName + GEOMETRY_SHADER);

		const char* vertPath = vertPathStr.c_str();
		const char* fragPath = fragPathStr.c_str();
		const char* geomPath = geomPathStr.c_str();

		return new Shader(vertPath, fragPath, geomPath);
	}

}}}