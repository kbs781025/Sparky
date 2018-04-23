#pragma once
#include "../shaders.h"
#include <string>

namespace sparky { namespace graphics { namespace ShaderFactory {

	Shader* BasicLightShader();
	Shader* NormalShader();
	Shader* DebugShader();
	Shader* CreateShadowShader();
	Shader* ApplyShadowShader();
	Shader* ModelShader();

	Shader* CreateShader(const std::string & shaderName);

}}}