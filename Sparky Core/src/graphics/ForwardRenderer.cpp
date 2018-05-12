#include <GL/glew.h>

#include "../platform/opengl/GLCommon.h"

#include "ForwardRenderer.h"
#include "UniformBuffer.h"
#include "Light.h"
#include "shaders.h"
#include "mesh.h"

namespace sparky { namespace graphics {

	enum VSMatUniformIndices
	{
		VSUniformIndex_ProjectionMatrix = 0,
		VSUniformIndex_ViewMatrix = 1,
		VSUniformIndex_ModelMatrix = 2,
		VSMatUniformIndex_Size
	};

	enum VSVecUniformIndices
	{
		VSUniformIndex_CameraPosition = 0,
		VSVecUniformIndex_Size
	};

	enum FSLightUniformIndices
	{
		FSUniformIndex_Lights = 0,
		FSLightUniformIndex_Size
	};

	ForwardRenderer::ForwardRenderer()
		:
		m_VSMatUniformBuffer(nullptr),
		m_VSVecUniformBuffer(nullptr),
		m_FSLightUniformBuffer(nullptr)
	{
		setScreenBufferSize(800, 600);
		init();
	}

	ForwardRenderer::ForwardRenderer(unsigned int width, unsigned int height)
		:
		m_VSMatUniformBuffer(nullptr),
		m_VSVecUniformBuffer(nullptr),
		m_FSLightUniformBuffer(nullptr)
	{
		setScreenBufferSize(width, height);
		init();
	}

	void ForwardRenderer::init()
	{
		m_CommandQueue.reserve(1000);

		m_VSMatUniformBuffer = new UniformBuffer(sizeof(glm::mat4) * VSMatUniformIndex_Size);
		m_VSVecUniformBuffer = new UniformBuffer(sizeof(glm::vec4) * VSVecUniformIndex_Size);
		m_FSLightUniformBuffer = new UniformBuffer(sizeof(Light) * FSLightUniformIndex_Size); // 1 point light at the moment

		m_MatrixData.resize(VSMatUniformIndex_Size);
		m_VecData.resize(VSVecUniformIndex_Size);
		m_LightData.resize(FSLightUniformIndex_Size);
	}

	void ForwardRenderer::begin()
	{
		GLCall(glViewport(0, 0, m_ScreenWidth, m_ScreenHeight));

		m_CommandQueue.clear();
		m_SystemUniforms.clear();
	}

	void ForwardRenderer::beginScene(const Camera * camera)
	{
		m_MatrixData[VSUniformIndex_ProjectionMatrix] = camera->getProjectionMatrix();
		m_MatrixData[VSUniformIndex_ViewMatrix] = camera->getViewMatrix();

		m_VecData[VSUniformIndex_CameraPosition] = glm::vec4(camera->getCameraPosition(), 0.0f);
	}

	void ForwardRenderer::submit(const RenderCommand & command)
	{
		m_CommandQueue.push_back(command);
	}

	void ForwardRenderer::submitMesh(const Mesh * mesh, const glm::mat4 & transform)
	{
		RenderCommand command;
		command.mesh = mesh;
		command.transform = transform;
		// TODO : link shader to mesh
		command.shader = nullptr;
		submit(command);
	}

	void ForwardRenderer::submitMesh(const Mesh * mesh, const glm::mat4 & transform, Shader * shader) 
	{
		RenderCommand command;
		command.mesh = mesh;
		command.transform = transform;
		command.shader = shader;
		submit(command);
	}

	// TODO : increase light capacity from 1 to as many as possible
	void ForwardRenderer::submitLightSetup(const std::vector<Light> & light)
	{
		assert(light.size() == 1);
		m_LightData = light;
	}

	void ForwardRenderer::endScene()
	{
	}

	void ForwardRenderer::end()
	{
	}

	void ForwardRenderer::present()
	{
		for (const auto& command : m_CommandQueue)
		{
			m_MatrixData[VSUniformIndex_ModelMatrix] = command.transform;
			setSystemUniforms(command.shader);
			//command.mesh->Render(*this);
			command.mesh->Draw(*command.shader);
		}
	}

	void ForwardRenderer::setSystemUniforms(const Shader * shader)
	{
		shader->enable();

		// TODO : bindUniformBlock should be in Init Method for performance
		unsigned int bindingPoint = 0;
		// TODO : flexible matrices uniform block name
		shader->bindUniformBlock("Matrices", bindingPoint);
		m_VSMatUniformBuffer->setBindingPoint(bindingPoint++);
		m_VSMatUniformBuffer->setUniformBlockData(m_MatrixData);

		// TODO : flexible matrices uniform block name
		shader->bindUniformBlock("Vectors", bindingPoint);
		m_VSVecUniformBuffer->setBindingPoint(bindingPoint++);
		m_VSVecUniformBuffer->setUniformBlockData(m_VecData);

		// TODO : flexible matrices uniform block name
		shader->bindUniformBlock("Lights", bindingPoint);
		m_FSLightUniformBuffer->setBindingPoint(bindingPoint++);
		m_FSLightUniformBuffer->setUniformBlockData(m_LightData);
	}

} }


