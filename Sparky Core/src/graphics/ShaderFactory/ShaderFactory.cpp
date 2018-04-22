#include "ShaderFactory.h"
#include <string>
namespace sparky { namespace graphics { namespace ShaderFactory {

	const char* BasicLightShaderVert = "../Shaders/BasicLightShader.vert";
	const char* BasicLightShaderFrag = "../Shaders/BasicLightShader.frag";
	const char* DebugShaderVert = "../Shaders/DebugShader.vert";
	const char* DebugShaderFrag = "../Shaders/DebugShader.frag";

	Shader * ShaderFactory::BasicLightShader()
	{
		return new Shader(BasicLightShaderVert, BasicLightShaderFrag);
	}

	Shader * ShaderFactory::DebugShader()
	{
		return new Shader(DebugShaderVert, DebugShaderFrag);
	}

}}}