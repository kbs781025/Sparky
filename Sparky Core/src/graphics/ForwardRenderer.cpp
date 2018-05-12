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
		m_VSVecUniformBuffer = new UniformBuffer(sizeof(glm::vec3) * VSVecUniformIndex_Size);
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

		m_VecData[VSUniformIndex_CameraPosition] = camera->getCameraPosition();
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

	// TODO : light setting method
	void ForwardRenderer::submitLightSetup(const Light & light)
	{
		
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
			command.mesh->Render(*this);
		}
	}

	void ForwardRenderer::setSystemUniforms(const Shader * shader)
	{
		// TODO : flexible matrices uniform block name
		unsigned int matrixBindingPoint = shader->getBlockBindingPoint("Matrices");
		m_VSMatUniformBuffer->setBindingPoint(matrixBindingPoint);
		m_VSMatUniformBuffer->setUniformBlockData(m_MatrixData);

		// TODO : flexible matrices uniform block name
		unsigned int vecBindingPoint = shader->getBlockBindingPoint("Vectors");
		m_VSVecUniformBuffer->setBindingPoint(vecBindingPoint);
		m_VSVecUniformBuffer->setUniformBlockData(m_VecData);

		// TODO : flexible matrices uniform block name
		unsigned int lightBindingPoint = shader->getBlockBindingPoint("PointLights");
		m_FSLightUniformBuffer->setBindingPoint(lightBindingPoint);
		m_FSLightUniformBuffer->setUniformBlockData(m_LightData);
	}

} }


