#include <GL/glew.h>

#include "../../res/shaders/preamble.glsl"

#include "../platform/opengl/GLCommon.h"

#include "ForwardRenderer.h"
#include "UniformBuffer.h"
#include "Light.h"
#include "shaders.h"
#include "mesh.h"


namespace sparky { namespace graphics {
	// TODO : Currently uniformblock causes for entire program to recompile. 
	// Should find flexible way to do this
	enum VSMatUniformIndices
	{
		VSUniformIndex_ProjectionMatrix = 0,
		VSUniformIndex_ViewMatrix = 1,
		VSUniformIndex_ModelMatrix = 2,
		VSUniformIndex_MVPMatrix = 3,
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
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClearDepth(1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glEnable(GL_DEPTH_TEST));
		GLCall(glEnable(GL_CULL_FACE));

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

	void ForwardRenderer::submitMesh(const Mesh * mesh, const glm::mat4 & transform, const Shader * shader) 
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
		m_LightData = std::move(light);
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
			m_MatrixData[VSUniformIndex_MVPMatrix] = m_MatrixData[VSUniformIndex_ProjectionMatrix] * 
				m_MatrixData[VSUniformIndex_ViewMatrix] * m_MatrixData[VSUniformIndex_ModelMatrix];
			setSystemUniforms(command.shader);
			//command.mesh->Render(*this);
			// TODO : temporary method
			command.mesh->Draw(*command.shader);
		}
	}

	void ForwardRenderer::setSystemUniforms(const Shader * shader)
	{
		shader->enable();

		// TODO : flexible matrices uniform block name
		m_VSMatUniformBuffer->setBindingPoint(MVP_UNIFORM_BLOCK_BINDING);
		m_VSMatUniformBuffer->setUniformBlockData(m_MatrixData);

		// TODO : flexible matrices uniform block name
		m_VSVecUniformBuffer->setBindingPoint(VEC_UNIFORM_BLOCK_BINDING);
		m_VSVecUniformBuffer->setUniformBlockData(m_VecData);

		// TODO : flexible matrices uniform block name
		m_FSLightUniformBuffer->setBindingPoint(LIGHT_UNIFORM_BLOCK_BINDING);
		m_FSLightUniformBuffer->setUniformBlockData(m_LightData);

		shader->setUniform3f(LIGHT_POSITION_LOCATION, glm::vec3(m_LightData[0].position));
	}

} }


