#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/ShaderFactory/ShaderFactory.h"
#include "src/graphics/Texture2D.h"
#include "src/graphics/TextureCube.h"
#include "src/graphics/TextureDepth.h"
#include "src/graphics/model.h"
#include "src/graphics/VertexArray.h"
#include "src/graphics/FramebufferDepth.h"
#include "src/graphics/Light.h"
#include "src/graphics/UniformBuffer.h"
#include "src/platform/opengl/GLCommon.h"
#include <GLFW/glfw3.h>

#include <time.h>
#include <vector>
#include <sstream>

void moveLightPosition(glm::vec3& lightPos)
{
	lightPos.x = sin(glfwGetTime());
	lightPos.y = cos(glfwGetTime());
}

void turnLightColor(glm::vec3& lightColor)
{
	lightColor.x = sin(glfwGetTime() * 2.0f);
	lightColor.y = sin(glfwGetTime() * 0.7f);
	lightColor.z = sin(glfwGetTime() * 1.3f);
}

void setPoint(const glm::vec3& point, const glm::vec3& color, std::vector<float>& vector, int index)
{
	int pointIndex = index * 6;
	vector[pointIndex + 0] = point.x;
	vector[pointIndex + 1] = point.y;
	vector[pointIndex + 2] = point.z;
	vector[pointIndex + 3] = color.x;
	vector[pointIndex + 4] = color.y;
	vector[pointIndex + 5] = color.z;
}

void setPointLightUniforms(sparky::graphics::Shader* const pShader, int index, const glm::vec3& position, 
							const glm::vec3& ambient, const glm::vec3& diffuse, const glm::vec3& specular, 
							float constant, float linear, float quadratic)
{
	std::string leftStr = "pointLights[";

	pShader->setUniform3f(leftStr.append(std::to_string(index)).append("].position").c_str(), position);
	leftStr = "pointLights[";

	pShader->setUniform3f(leftStr.append(std::to_string(index)).append("].ambient").c_str(), ambient);
	leftStr = "pointLights[";

	pShader->setUniform3f(leftStr.append(std::to_string(index)).append("].diffuse").c_str(), diffuse);
	leftStr = "pointLights[";

	pShader->setUniform3f(leftStr.append(std::to_string(index)).append("].specular").c_str(), specular);
	leftStr = "pointLights[";

	pShader->setUniform1f(leftStr.append(std::to_string(index)).append("].constant").c_str(), constant);
	leftStr = "pointLights[";

	pShader->setUniform1f(leftStr.append(std::to_string(index)).append("].linear").c_str(), linear);
	leftStr = "pointLights[";

	pShader->setUniform1f(leftStr.append(std::to_string(index)).append("].quadratic").c_str(), quadratic);
}

sparky::graphics::VertexArray* pCubeVao;
sparky::graphics::VertexArray* pQuadVao;
sparky::graphics::VertexArray* pPlaneVao;
sparky::graphics::VertexArray* pSkyBoxVao;

void initCube()
{
	using namespace sparky;
	using namespace graphics;

	float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		
		// front face
		 -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 -1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		 -1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		
		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		
		// right face
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     
		
		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		
		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left  
		};

	VertexBufferContext context(GL_STATIC_DRAW, vertices, sizeof(vertices), BufferLayout::getPosNormTexLayout());
	pCubeVao = new VertexArray(context);
}
void renderCube()
{
	pCubeVao->Draw();
}
void initQuad()
{
	using namespace sparky;
	using namespace graphics;

	//float quadVertices[] = {
	//	// positions        // texture Coords
	//	-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	//	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	//	1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
	//	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	//};
	glm::vec3 pos1(-1.0f, 1.0f, 0.0f);
	glm::vec3 pos2(-1.0f, -1.0f, 0.0f);
	glm::vec3 pos3(1.0f, -1.0f, 0.0f);
	glm::vec3 pos4(1.0f, 1.0f, 0.0f);
	// texture coordinates
	glm::vec2 uv1(0.0f, 1.0f);
	glm::vec2 uv2(0.0f, 0.0f);
	glm::vec2 uv3(1.0f, 0.0f);
	glm::vec2 uv4(1.0f, 1.0f);
	// normal vector
	glm::vec3 nm(0.0f, 0.0f, 1.0f);

	// calculate tangent/bitangent vectors of both triangles
	glm::vec3 tangent1, bitangent1;
	glm::vec3 tangent2, bitangent2;
	// triangle 1
	// ----------
	glm::vec3 edge1 = pos2 - pos1;
	glm::vec3 edge2 = pos3 - pos1;
	glm::vec2 deltaUV1 = uv2 - uv1;
	glm::vec2 deltaUV2 = uv3 - uv1;

	GLfloat f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent1.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent1.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent1.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent1 = glm::normalize(tangent1);

	bitangent1.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent1.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent1.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent1 = glm::normalize(bitangent1);

	// triangle 2
	// ----------
	edge1 = pos3 - pos1;
	edge2 = pos4 - pos1;
	deltaUV1 = uv3 - uv1;
	deltaUV2 = uv4 - uv1;

	f = 1.0f / (deltaUV1.x * deltaUV2.y - deltaUV2.x * deltaUV1.y);

	tangent2.x = f * (deltaUV2.y * edge1.x - deltaUV1.y * edge2.x);
	tangent2.y = f * (deltaUV2.y * edge1.y - deltaUV1.y * edge2.y);
	tangent2.z = f * (deltaUV2.y * edge1.z - deltaUV1.y * edge2.z);
	tangent2 = glm::normalize(tangent2);


	bitangent2.x = f * (-deltaUV2.x * edge1.x + deltaUV1.x * edge2.x);
	bitangent2.y = f * (-deltaUV2.x * edge1.y + deltaUV1.x * edge2.y);
	bitangent2.z = f * (-deltaUV2.x * edge1.z + deltaUV1.x * edge2.z);
	bitangent2 = glm::normalize(bitangent2);


	float quadVertices[] = {
		// positions            // texcoords  // normal         // tangent                          // bitangent
		pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos2.x, pos2.y, pos2.z, uv2.x, uv2.y, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,
		pos3.x, pos3.y, pos3.z, uv3.x, uv3.y, nm.x, nm.y, nm.z, tangent1.x, tangent1.y, tangent1.z, bitangent1.x, bitangent1.y, bitangent1.z,

		pos1.x, pos1.y, pos1.z, uv1.x, uv1.y, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos3.x, pos3.y, pos3.z, uv3.x, uv3.y, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z,
		pos4.x, pos4.y, pos4.z, uv4.x, uv4.y, nm.x, nm.y, nm.z, tangent2.x, tangent2.y, tangent2.z, bitangent2.x, bitangent2.y, bitangent2.z
	};
	BufferLayout layout;
	layout.PushPosition();
	layout.PushTexCoord();
	layout.PushNormal();
	layout.PushTangent();
	layout.PushBinormal();
	VertexBufferContext context(GL_STATIC_DRAW, quadVertices, sizeof(quadVertices), layout);
	pQuadVao = new VertexArray(context);
}
void renderQuad()
{
	pQuadVao->Draw();
}
void initPlane()
{
	using namespace sparky;
	using namespace graphics;

	float planeVertices[] = {
		// positions            // normals         // texcoords
		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
	};

	VertexBufferContext context = VertexBufferContext(GL_STATIC_DRAW, planeVertices, sizeof(planeVertices), BufferLayout::getPosNormTexLayout());
	pPlaneVao = new VertexArray(context);
}
void drawPlane()
{
	pPlaneVao->Draw();
}
void renderScene(sparky::graphics::Shader &shader)
{
	// floor
	glm::mat4 model;
	shader.setUniformMat4("model", model);
	GLCall(glDisable(GL_CULL_FACE));
	pPlaneVao->Draw();
	GLCall(glEnable(GL_CULL_FACE));
	// cubes
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setUniformMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(2.0f, 0.0f, 1.0f));
	model = glm::scale(model, glm::vec3(0.5f));
	shader.setUniformMat4("model", model);
	renderCube();
	model = glm::mat4();
	model = glm::translate(model, glm::vec3(-1.0f, 0.0f, 2.0f));
	model = glm::rotate(model, glm::radians(60.0f), glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	model = glm::scale(model, glm::vec3(0.25));
	shader.setUniformMat4("model", model);
	renderCube();
}
void initSkyBox()
{
	using namespace sparky;
	using namespace graphics;

	float skyboxVertices[] = {
		// positions          
		-10.0f,  10.0f, -10.0f,
		-10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f, -10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,

		-10.0f, -10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f, -10.0f,  10.0f,
		-10.0f, -10.0f,  10.0f,

		-10.0f,  10.0f, -10.0f,
		10.0f,  10.0f, -10.0f,
		10.0f,  10.0f,  10.0f,
		10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f,  10.0f,
		-10.0f,  10.0f, -10.0f,

		-10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f, -10.0f,
		10.0f, -10.0f, -10.0f,
		-10.0f, -10.0f,  10.0f,
		10.0f, -10.0f,  10.0f
	};
	
	VertexBufferContext context(GL_STATIC_DRAW, skyboxVertices, sizeof(skyboxVertices), BufferLayout::getPosLayout());
	pSkyBoxVao = new VertexArray(context);
}
void drawSkyBox()
{
	pSkyBoxVao->Draw();
}

int main()
{
	using namespace sparky;
	using namespace graphics;

	Window window("Sparky", 800, 600);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const double MS_PER_UPDATE = 1.0f / 60.0f;

	// State Setting
	GLCall(glEnable(GL_DEPTH_TEST));
	GLCall(glEnable(GL_PROGRAM_POINT_SIZE));
	GLCall(glEnable(GL_CULL_FACE));
	
	// Shader loading
	Shader* modelShader = ShaderFactory::ModelShader();
	Shader* normalDisplayShader = ShaderFactory::NormalShader();
	Shader* createShadowShader = ShaderFactory::CreateShadowShader();
	Shader* applyShadowShader = ShaderFactory::ApplyShadowShader();
	Shader* debug = ShaderFactory::CreateShader("debugShader");
	Shader* skyboxShader = ShaderFactory::CreateShader("cubeMapShader");
	Shader* normalMapShader = ShaderFactory::CreateShader("normalMappingShader");
	
	// Texture and model loading
	Model nanosuit("Texture/models/nanosuit/nanosuit.obj");
	Texture2D container = Texture2D("Texture/Images/container2.png");
	Texture2D brickWallDiffuse = Texture2D("Texture/Images/brickwall.jpg");
	Texture2D brickWallNormal = Texture2D("Texture/Images/brickwall_normal.jpg");
	std::vector<std::string> cubeMapFaces =
	{
		"Texture/Images/skybox/right.jpg",
		"Texture/Images/skybox/left.jpg",
		"Texture/Images/skybox/top.jpg",
		"Texture/Images/skybox/bottom.jpg",
		"Texture/Images/skybox/front.jpg",
		"Texture/Images/skybox/back.jpg"
	};
	TextureCube cubeMap = TextureCube("skybox", cubeMapFaces);

	// Light Setting
	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(2.0f, 4.0f, 2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	std::vector<PointLight> pointLights;
	pointLights.emplace_back(pointLightColor, pointLightPositions[0], glm::vec3(1.0f, 0.007f, 0.0002f));
	//pointLights.emplace_back(pointLightColor, pointLightPositions[1], glm::vec3(1.0f, 0.007f, 0.0002f));
	
	// Buffer Object Setting
	modelShader->bindUniformBlock("Matrices", 1);
	applyShadowShader->bindUniformBlock("Matrices", 1);
	normalMapShader->bindUniformBlock("Matrices", 1);
	normalDisplayShader->bindUniformBlock("Matrices", 1);
	UniformBuffer* pMatrixUBO = new UniformBuffer(1, sizeof(glm::mat4));
	glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);

	modelShader->bindUniformBlock("PointLights", 2);
	normalMapShader->bindUniformBlock("PointLights", 2);
	UniformBuffer* pLightUBO = new UniformBuffer(2, pointLights.size() * sizeof(PointLight));
	pLightUBO->setUniformBlockData(pointLights);

	FrameBufferDepth depthFBO(SHADOW_WIDTH, SHADOW_HEIGHT);

	initCube();
	initQuad();
	initPlane();
	initSkyBox();
	
	Timer timer, t;
	unsigned int frame = 0;

	double time = 0.0f;
	double lag = 0.0;
	double prevClock = glfwGetTime();

	while (!window.closed())
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		window.clear();

		// Update Block
		double currentClock = glfwGetTime();
		double frameTime = currentClock - prevClock;
		window.processInput(frameTime);
		prevClock = currentClock;
		lag += frameTime;
		while (lag >= MS_PER_UPDATE)
		{
			window.update();
			lag -= MS_PER_UPDATE;
		}

		// FPS Timer block
		{
			frame++;
			if (t.cyclicTimer(1.0f))
			{
				printf("%d frame\n", frame);
				frame = 0;
			}
		}

		//Render Block
		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
		modelShader->enable();
		modelShader->setUniformMat4("model", model);
		glm::mat4 view = window.getViewMatrix();
		modelShader->setUniform3f("viewPos", window.getCamPosition());
		std::vector<glm::mat4> matrices;
		matrices.emplace_back(projection * view * model);
		pMatrixUBO->setUniformBlockData(matrices);
		nanosuit.Draw(*modelShader);

		GLCall(glDepthFunc(GL_LEQUAL));
		skyboxShader->enable();
		skyboxShader->setUniformMat4("projection", projection);
		glm::mat4 cubeMapView = glm::mat4(glm::mat3(view));
		skyboxShader->setUniformMat4("view", cubeMapView);
		cubeMap.bind();
		drawSkyBox();
		GLCall(glDepthFunc(GL_LESS)); 

		/*depthFBO.bind();
		createShadowShader->enable();
		glm::mat4 lightView = glm::lookAt(pointLightPositions[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		createShadowShader->setUniformMat4("lightView", lightView);
		createShadowShader->setUniformMat4("lightProjection", lightProjection);
		renderScene(*createShadowShader);

		depthFBO.unBind();
		GLCall(glViewport(0, 0, window.getWidth(), window.getHeight()));
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		applyShadowShader->enable();
		applyShadowShader->setUniformMat4("lightSpaceMatrix", lightProjection * lightView);
		applyShadowShader->setUniform3f("lightPosition", pointLightPositions[0]);
		applyShadowShader->setUniform1i("diffuseMap", 0);
		applyShadowShader->setUniform1i("shadowMap", 1);
		container.bind();
		depthFBO.getTextureDepth()->bind(1);
		renderScene(*applyShadowShader);*/
		/*normalDisplayShader->enable();
		normalDisplayShader->setUniformMat4("model", model);
		nanosuit.Draw(*normalDisplayShader, false);*/
		
		/*normalMapShader->enable();
		normalMapShader->setUniformMat4("model", model);
		normalMapShader->setUniform3f("worldLightPos", pointLightPositions[0]);
		normalMapShader->setUniform3f("worldViewPos", window.getCamPosition());
		normalMapShader->setUniform1i("material.texture_diffuse1", 0);
		normalMapShader->setUniform1i("material.texture_normal1", 1);
		normalMapShader->setUniform1f("material.shininess", 32);
		brickWallDiffuse.bind(0);
		brickWallNormal.bind(1);
		renderQuad();

		GLCall(glDepthFunc(GL_LEQUAL));
		skyboxShader->enable();
		skyboxShader->setUniformMat4("projection", projection);
		glm::mat4 cubeMapView = glm::mat4(glm::mat3(view));
		skyboxShader->setUniformMat4("view", cubeMapView);
		cubeMap.bind();
		drawSkyBox();
		GLCall(glDepthFunc(GL_LESS));*/

		glfwSwapBuffers(window.getWindow());
	}

	return 0;
}


