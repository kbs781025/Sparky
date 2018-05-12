#pragma once

#include <vector>

#include "Renderer3D.h"

namespace sparky { namespace graphics {

	class Shader;
	class UniformBuffer;

	class ForwardRenderer : public Renderer3D
	{
	private:
		UniformBuffer* m_VSMatUniformBuffer;
		UniformBuffer* m_VSVecUniformBuffer;
		UniformBuffer* m_FSLightUniformBuffer;

		std::vector<glm::mat4> m_MatrixData;
		std::vector<glm::vec3> m_VecData;
		std::vector<Light> m_LightData;
	public:
		ForwardRenderer();
		ForwardRenderer(unsigned int width, unsigned int height);

		void init() override;
		void begin() override;
		void beginScene(const Camera* camera) override;
		void submit(const RenderCommand& command) override;
		void submitMesh(const Mesh* mesh, const glm::mat4& transform) override;
		void submitLightSetup(const Light & light);
		void endScene() override;
		void end() override;
		void present() override;
	private:
		void setSystemUniforms(const Shader* shader);
	};

}}