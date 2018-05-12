#pragma once

#include <string>
#include <vector>

#include "../maths/maths.h"

namespace sparky { namespace graphics {

	class Mesh;
	class Shader;

	struct RendererUniform
	{
		std::string uniform;
		unsigned char* value;
	};

	struct RenderCommand
	{
		const Mesh* mesh;
		const Shader* shader;
		//std::vector<RendererUniform> uniforms;
		glm::mat4 transform;
	};

}}