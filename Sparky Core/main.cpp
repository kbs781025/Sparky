#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/Texture2D.h"
#include "src/graphics/TextureCube.h"
#include "src/graphics/TextureDepth.h"
#include "src/graphics/model.h"
#include "src/graphics/VertexArray.h"
#include "src/graphics/FramebufferDepth.h"
#include "src/graphics/Framebuffer2D.h"
#include "src/graphics/Light.h"
#include "src/graphics/UniformBuffer.h"
#include "src/platform/opengl/GLCommon.h"
#include "src/graphics/ForwardRenderer.h"
#include "src/graphics/ShaderSet.h"

#include <GL/glew.h>
#include <time.h>
#include <GLFW/glfw3.h>
#include <vector>

#define PRIMITIVES

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

	Window window("Sparky", 720, 560);
	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
	const double MS_PER_UPDATE = 1.0f / 60.0f;

	// Shader loading
	ShaderSet shaderset;
	shaderset.SetVersion("450");
	shaderset.SetPreambleFile("preamble.glsl");
	const Shader* normalMapShader = shaderset.AddProgramFromExts({ "normalMappingShader.vert", "normalMappingShader.frag" });
	const Shader* hdrShader = shaderset.AddProgramFromExts({ "hdrShader.vert", "hdrShader.frag" });
	const Shader* bloomShader = shaderset.AddProgramFromExts({ "hdrShader.vert", "bloomShader.frag" });
	const Shader* defferedGeoPassShader = shaderset.AddProgramFromExts({ "DefferedGPass.vert", "DefferedGPass.frag" });
	shaderset.UpdatePrograms();

	#ifdef DEBUG
	Shader* normalDisplayShader = ShaderFactory::NormalShader();
	Shader* debug = ShaderFactory::CreateShader("debugShader");
	#endif
	
	#ifdef SKYBOX
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
	Shader* skyboxShader = ShaderFactory::CreateShader("cubeMapShader");
	initSkyBox();
	#endif

	#ifdef SHADOWMAP
	Shader* createShadowShader = ShaderFactory::CreateShadowShader();
	Shader* applyShadowShader = ShaderFactory::ApplyShadowShader();
	FrameBufferDepth depthFBO(SHADOW_WIDTH, SHADOW_HEIGHT);
	#endif

	#ifdef PRIMITIVES
	initCube();
	initQuad();
	initPlane();
	#endif

	// Texture and model loading
	Model man("res/Texture/models/man/muro.obj");

	// Light Setting
	glm::vec3 pointLightPositions = glm::vec3(2.0f, 4.0f, 2.0f);
	glm::vec3 lightDir = glm::vec3(-0.5f, -0.5f, -0.5f);
	glm::vec4 pointLightColor = glm::vec4(10.0f, 10.0f, 10.0f, 5.0f);

	std::vector<Light> Lights;
	Lights.emplace_back(pointLightColor, pointLightPositions, lightDir, glm::vec3(1.0f, 0.007f, 0.0002f));

	glm::mat4 model;
	model = glm::scale(model, glm::vec3(0.05f));

	// Renderer
	ForwardRenderer* renderer = new ForwardRenderer(window.getWidth(), window.getHeight());
	renderer->init();

	// HDR FrameBuffer
	unsigned int hdrFBO;
	GLCall(glGenFramebuffers(1, &hdrFBO));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO));

	unsigned int colorBuffers[2];
	GLCall(glGenTextures(2, colorBuffers));
	for (int i = 0; i < 2; ++i)
	{
		GLCall(glBindTexture(GL_TEXTURE_2D, colorBuffers[i]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.getWidth(), window.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0));
	}

	unsigned int attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);

	unsigned int rboDepth;
	GLCall(glGenRenderbuffers(1, &rboDepth));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, rboDepth));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window.getWidth(), window.getHeight()));
	
	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error" << std::endl;
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	unsigned int tempFBO[2];
	GLCall(glGenFramebuffers(2, tempFBO));
	unsigned int tempColorBuffer[2];
	GLCall(glGenTextures(2, tempColorBuffer));

	for (int i = 0; i < 2; i++)
	{
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, tempFBO[i]));
		GLCall(glBindTexture(GL_TEXTURE_2D, tempColorBuffer[i]));
		GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, window.getWidth(), window.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));
		GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tempColorBuffer[i], 0));
		if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		{
			std::cout << "Framebuffer error" << std::endl;
		}
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));

	// GBuffer
	unsigned int GBuffer;
	GLCall(glGenFramebuffers(1, &GBuffer));
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, GBuffer));

	unsigned int GBufferTextures[3]; //POS, NORMAL, ALBEDO, SPECULAR
	GLCall(glGenTextures(3, GBufferTextures));

	//POS Texture
	GLCall(glBindTexture(GL_TEXTURE_2D, GBufferTextures[0]));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, window.getWidth(), window.getHeight(), 0, GL_RGB, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, GBufferTextures[0], 0));

	// Normal Texture
	GLCall(glBindTexture(GL_TEXTURE_2D, GBufferTextures[1]));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, window.getWidth(), window.getHeight(), 0, GL_RGB, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, GBufferTextures[1], 0));

	// Albedo + Specular Texture
	GLCall(glBindTexture(GL_TEXTURE_2D, GBufferTextures[2]));
	GLCall(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, window.getWidth(), window.getHeight(), 0, GL_RGBA, GL_FLOAT, NULL));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
	GLCall(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
	GLCall(glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, GBufferTextures[2], 0));

	unsigned int GBufferAttachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, GBufferAttachments);

	unsigned int GBufferDepth;
	GLCall(glGenRenderbuffers(1, &GBufferDepth));
	GLCall(glBindRenderbuffer(GL_RENDERBUFFER, GBufferDepth));
	GLCall(glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, window.getWidth(), window.getHeight()));

	GLCall(glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, GBufferDepth));
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Framebuffer error" << std::endl;
	}
	GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
	
	Timer timer, t;
	unsigned int frame = 0;

	double time = 0.0f;
	double lag = 0.0;
	double prevClock = glfwGetTime();
	
	while (!window.closed())
	{
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
		shaderset.UpdatePrograms();
		renderer->begin();
		renderer->beginScene(window.getCamera()); 
		renderer->submitLightSetup(Lights);
		man.SubmitMesh(renderer, model, defferedGeoPassShader);
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, GBuffer));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		renderer->present();
		renderer->endScene();
		renderer->end();

		// Blitting 4 screen
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		GLCall(glBindFramebuffer(GL_READ_FRAMEBUFFER, GBuffer));
		GLuint halfWidth = window.getWidth() / 2.0f;
		GLuint halfHeight = window.getHeight() / 2.0f;

		GLCall(glReadBuffer(GL_COLOR_ATTACHMENT0));
		GLCall(glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), 0, halfHeight, halfWidth, window.getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR));

		GLCall(glReadBuffer(GL_COLOR_ATTACHMENT1));
		GLCall(glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), halfWidth, halfHeight, window.getWidth(), window.getHeight(), GL_COLOR_BUFFER_BIT, GL_LINEAR));

		GLCall(glReadBuffer(GL_COLOR_ATTACHMENT2));
		GLCall(glBlitFramebuffer(0, 0, window.getWidth(), window.getHeight(), 0, 0, halfWidth, halfHeight, GL_COLOR_BUFFER_BIT, GL_LINEAR));

		/*bloomShader->enable();
		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, tempFBO[0]));
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, colorBuffers[1]));
		renderQuad();

		for (int i = 0; i < 10; ++i)
		{
			static bool horizontal = true;
			bloomShader->setUniform1i("horizontal", horizontal);
			GLCall(glBindFramebuffer(GL_FRAMEBUFFER, tempFBO[horizontal]));
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			GLCall(glActiveTexture(GL_TEXTURE0));
			GLCall(glBindTexture(GL_TEXTURE_2D, tempColorBuffer[!horizontal]));
			renderQuad();
			horizontal = !horizontal;
		}

		GLCall(glBindFramebuffer(GL_FRAMEBUFFER, 0));
		GLCall(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
		hdrShader->enable();
		GLCall(glActiveTexture(GL_TEXTURE0));
		GLCall(glBindTexture(GL_TEXTURE_2D, colorBuffers[0]));
		GLCall(glActiveTexture(GL_TEXTURE1));
		GLCall(glBindTexture(GL_TEXTURE_2D, tempColorBuffer[0]));
		hdrShader->setUniform1f("exposure", 1.0);
		renderQuad();*/
		
		#ifdef GARBAGE
		/*GLCall(glDepthFunc(GL_LEQUAL));
		skyboxShader->enable();
		skyboxShader->setUniformMat4("projection", projection);
		glm::mat4 cubeMapView = glm::mat4(glm::mat3(view));
		skyboxShader->setUniformMat4("view", cubeMapView);
		cubeMap.bind();
		drawSkyBox();
		GLCall(glDepthFunc(GL_LESS)); */

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

		/*GLCall(glDepthFunc(GL_LEQUAL));
		skyboxShader->enable();
		skyboxShader->setUniformMat4("projection", projection);
		glm::mat4 cubeMapView = glm::mat4(glm::mat3(view));
		skyboxShader->setUniformMat4("view", cubeMapView);
		cubeMap.bind();
		drawSkyBox();
		GLCall(glDepthFunc(GL_LESS));*/
		#endif

		glfwSwapBuffers(window.getWindow());
	}

	return 0;
}


