#pragma once

#include <vector>

#include "RenderCommand.h"
#include "camera.h"

namespace sparky { namespace graphics {

	typedef std::vector<RenderCommand> CommandQueue;
	typedef std::vector<RendererUniform> SystemUniformList;

	struct Light;

	class Renderer3D
	{
	protected:
		unsigned int m_ScreenWidth, m_ScreenHeight;
		CommandQueue m_CommandQueue;
		SystemUniformList m_SystemUniforms;
	public:
		virtual ~Renderer3D() {};

		virtual void init() = 0;
		virtual void begin() = 0;
		virtual void beginScene(const Camera* camera) = 0;
		virtual void submit(const RenderCommand& command) = 0;
		virtual void submitMesh(const Mesh* mesh, const glm::mat4& transform) = 0;
		virtual void submitLightSetup(const std::vector<Light>& light) = 0;
		virtual void endScene() = 0;
		virtual void end() = 0;
		virtual void present() = 0;

		virtual void setScreenBufferSize(unsigned int width, unsigned int height) { m_ScreenWidth = width; m_ScreenHeight = height; }
	};

}}