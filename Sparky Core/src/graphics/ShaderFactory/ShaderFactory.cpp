#include "ShaderFactory.h"
#include <string>
namespace sparky { namespace graphics { namespace ShaderFactory {

	const char* BasicLightShaderVert = "src/graphics/ShaderFactory/Shaders/basicLightShader.vert";
	const char* BasicLightShaderFrag = "src/graphics/ShaderFactory/Shaders/basicLightShader.frag";
	const char* DebugShaderVert = "src/graphics/ShaderFactory/Shaders/debugShader.vert";
	const char* DebugShaderFrag = "src/graphics/ShaderFactory/Shaders/debugShader.frag";
	const char* NormalShaderVert = "src/graphics/ShaderFactory/Shaders/normalVecShader.vert";
	const char* NormalShaderFrag = "src/graphics/ShaderFactory/Shaders/normalVecShader.frag";
	const char* NormalShaderGeom = "src/graphics/ShaderFactory/Shaders/normalVecShader.geom";

	Shader * ShaderFactory::BasicLightShader()
	{
		return new Shader(BasicLightShaderVert, BasicLightShaderFrag);
	}

	Shader * ShaderFactory::NormalShader()
	{
		return new Shader(NormalShaderVert, NormalShaderFrag, NormalShaderGeom);
	}

	Shader * ShaderFactory::DebugShader()
	{
		return new Shader(DebugShaderVert, DebugShaderFrag);
	}

}}}