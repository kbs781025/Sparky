#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/Texture2D.h"
#include "src/graphics/TextureCube.h"
#include "src/graphics/model.h"
#include "src/graphics/ShaderFactory/ShaderFactory.h"
#include "src/graphics/TextureDepth.h"
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

unsigned int cubeVAO = 0;
unsigned int cubeVBO = 0;
void renderCube()
{
	// initialize (if necessary)
	if (cubeVAO == 0)
	{
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

		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
	// render Cube
	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

// renderQuad() renders a 1x1 XY quad in NDC
// -----------------------------------------
unsigned int quadVAO = 0;
unsigned int quadVBO;
unsigned int planeVAO;
void renderQuad()
{
	if (quadVAO == 0)
	{
		float quadVertices[] = {
			// positions        // texture Coords
			-1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
			1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
			1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		};
		// setup plane VAO
		glGenVertexArrays(1, &quadVAO);
		glGenBuffers(1, &quadVBO);
		glBindVertexArray(quadVAO);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	}
	glBindVertexArray(quadVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glBindVertexArray(0);
}

void renderScene(sparky::graphics::Shader &shader)
{
	// floor
	glm::mat4 model;
	shader.setUniformMat4("model", model);
	glBindVertexArray(planeVAO);
	glDisable(GL_CULL_FACE);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glEnable(GL_CULL_FACE);
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

#define NOSHOW
#define MODEL_IMPORT
#ifndef MODEL_IMPORT
int main()
{
	using namespace sparky;
	using namespace graphics;

	Window window("Sparky", 800, 600);

	Timer timer, t;
	unsigned int frame = 0;

	float cubeVertex[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // Bottom-left
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f, // bottom-right         
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f, // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f, // top-right
	    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f, // top-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left					  
		// Left face
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-right
		// Right face
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // bottom-right
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // top-left
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f, // top-left
	    0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, // top-right
		// Top face
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f, // top-right     
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f, // top-left
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f  // bottom-left      
	};

	float planeVertices[] = {
		// positions          // texture Coords (note we set these higher than 1 (together with GL_REPEAT as texture wrapping mode). this will cause the floor texture to repeat)
		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f,  5.0f,  0.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,

		5.0f, -0.5f,  5.0f,  2.0f, 0.0f,
		-5.0f, -0.5f, -5.0f,  0.0f, 2.0f,
		5.0f, -0.5f, -5.0f,  2.0f, 2.0f
	};

	float quadVertex[] =
	{
		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

		-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
		0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
		0.05f,  0.05f,  0.0f, 1.0f, 1.0f
	};

	float screenWidth = window.getWidth();
	float screenHeight = window.getHeight();
	float screenVertex1[] =
	{
		0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
		0.0f, 0.0f,  0.0f, 0.0f, 0.0f,
		1.0f, 0.0f,  0.0f, 1.0f, 0.0f,

		0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  0.0f, 1.0f, 1.0f
	};
	float points[] = {
		-0.5f,  0.5f, 1.0f, 0.0f, 0.0f, // top-left
		0.5f,  0.5f, 0.0f, 1.0f, 0.0f, // top-right
		0.5f, -0.5f, 0.0f, 0.0f, 1.0f, // bottom-right
		-0.5f, -0.5f, 1.0f, 1.0f, 0.0f  // bottom-left
	};

	std::vector<glm::vec3> screenPos;
	screenPos.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	screenPos.push_back(glm::vec3(-1.0f, 0.0f, 0.0f));
	screenPos.push_back(glm::vec3(-1.0f, -1.0f, 0.0f));
	screenPos.push_back(glm::vec3(0.0f, -1.0f, 0.0f));

	unsigned int indicies[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f,  0.001f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
	};

	/*unsigned int quadVAO, quadVBO;
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertex), quadVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(2 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glm::vec2 translations[100];
	int index = 0;
	float offset = 0.1f;
	for (int x = -10; x < 10; x += 2)
	{
		for (int y = -10; y < 10; y += 2)
		{
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2) * 100, translations, GL_STATIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (void*)0);
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glVertexAttribDivisor(2, 1);*/
	
	glBindVertexArray(0);

	unsigned int amount = 10000;
	glm::mat4* pModelMatrices = new glm::mat4[amount];
	srand(glfwGetTime());
	float radius = 50.0f;
	float offset = 2.5f;
	for (unsigned int i = 0; i < amount; i++)
	{
		glm::mat4 model;
		// 1. translation: displace along circle with 'radius' in range [-offset, offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 2. scale: Scale between 0.05 and 0.25f
		float scale = (rand() % 20) / 100.0f + 0.05;
		model = glm::scale(model, glm::vec3(scale));

		// 3. rotation: add random rotation around a (semi)randomly picked rotation axis vector
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 4. now add to list of matrices
		pModelMatrices[i] = model;
	}

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);

	Shader basicShader = Shader("src/shaders/basicShader.vert", "src/shaders/basicShader.frag");
	Shader instanceShader = Shader("src/shaders/instanceShader.vert", "src/shaders/instanceShader.frag");
	Model planet = Model("Texture/planet/planet.obj");
	Model rock = Model("Texture/rock/rock.obj");

	unsigned int instanceVBO;
	glGenBuffers(1, &instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
	GLsizei vec4Size = sizeof(glm::vec4);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::mat4) * amount, pModelMatrices, GL_STATIC_DRAW);

	for (unsigned int i = 0; i < rock.getMeshNum(); i++)
	{
		unsigned int meshVAO = rock.getMeshes()[i].getVAO();
		glBindVertexArray(meshVAO);
		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(vec4Size));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(2 * vec4Size));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * vec4Size, (void*)(3 * vec4Size));
		glEnableVertexAttribArray(6);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}
	
	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		window.clear();

		glm::mat4 view = window.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		
		basicShader.enable();
		basicShader.setUniformMat4("projection", projection);
		basicShader.setUniformMat4("view", view);
		
		glm::mat4 model;
		model = glm::translate(model, glm::vec3(0.0f, 3.0f, -4.0f));
		model = glm::scale(model, glm::vec3(4.0f));
		basicShader.setUniformMat4("model", model);
		planet.Draw(basicShader);

		instanceShader.enable();
		instanceShader.setUniformMat4("projection", projection);
		instanceShader.setUniformMat4("view", view);
		rock.DrawInstances(instanceShader, amount);

		float delta = timer.elapsed();
		window.update(delta);

		frame++;
		if (t.cyclicTimer(1.0f))
		{
			printf("%d frame\n", frame);
			frame = 0;
		}
	}

	return 0;
}
#else
int main()
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
	
	Window window("Sparky", 800, 600);

	unsigned int skyboxVAO, skyboxVBO;
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	float planeVertices[] = {
		// positions            // normals         // texcoords
		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,

		25.0f, -0.5f,  25.0f,  0.0f, 1.0f, 0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,   0.0f, 25.0f,
		25.0f, -0.5f, -25.0f,  0.0f, 1.0f, 0.0f,  25.0f, 10.0f
	};
	// plane VAO
	unsigned int planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);
	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindVertexArray(0);

	unsigned int uboBlock;
	glGenBuffers(1, &uboBlock);
	glBindBuffer(GL_UNIFORM_BUFFER, uboBlock);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, nullptr, GL_STATIC_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

	TextureDepth depthMap(SHADOW_WIDTH, SHADOW_HEIGHT);

	unsigned int depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap.getHandle(), 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_PROGRAM_POINT_SIZE);
	glEnable(GL_FRAMEBUFFER_SRGB);
	glEnable(GL_CULL_FACE);

	//Model nanosuit("Texture/models/nanosuit/nanosuit.obj");
	Shader* modelShader = ShaderFactory::ModelShader();
	Shader* normalDisplayShader = ShaderFactory::NormalShader();
	Shader* createShadowShader = ShaderFactory::CreateShadowShader();
	Shader* applyShadowShader = ShaderFactory::ApplyShadowShader();
	Shader* debug = ShaderFactory::CreateShader("debugShader");
	Shader* skyboxShader = ShaderFactory::CreateShader("cubeMapShader");
	Texture2D container = Texture2D("container", "Texture/Images/container2.png");

	modelShader->bindUniformBlock("Matrices", 1);
	applyShadowShader->bindUniformBlock("Matrices", 1);
	glBindBufferRange(GL_UNIFORM_BUFFER, 1, uboBlock, 0, sizeof(glm::mat4) * 2);
	glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, uboBlock);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

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
	
	skyboxShader->enable();
	skyboxShader->setUniform1i("cubemap", 0);

	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(-2.0f, 4.0f, -1.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	modelShader->enable();
	for (int i = 0; i < 4; i++)
	{
		setPointLightUniforms(modelShader, i, pointLightPositions[i],
							  pointLightColor * 0.2f , pointLightColor * 0.7f, pointLightColor * 1.0f,
							  1.0f, 0.007f, 0.0002f);
	}

	Timer timer, t;
	unsigned int frame = 0;

	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		window.clear();

		glm::mat4 model;
		//model = glm::scale(model, glm::vec3(0.2f));
		//model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));
		glm::mat4 view = window.getViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, uboBlock);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(view));
		glBindBuffer(GL_UNIFORM_BUFFER, 0);

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glCullFace(GL_FRONT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_DEPTH_BUFFER_BIT);
		createShadowShader->enable();
		glm::mat4 lightView = glm::lookAt(pointLightPositions[0], glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.0f, 7.5f);
		createShadowShader->setUniformMat4("lightView", lightView);
		createShadowShader->setUniformMat4("lightProjection", lightProjection);
		renderScene(*createShadowShader);

		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glViewport(0, 0, window.getWidth(), window.getHeight());
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		applyShadowShader->enable();
		applyShadowShader->setUniformMat4("lightSpaceMatrix", lightProjection * lightView);
		applyShadowShader->setUniform3f("lightPosition", pointLightPositions[0]);
		applyShadowShader->setUniform1i("diffuseMap", 0);
		applyShadowShader->setUniform1i("shadowMap", 1);
		container.bind();
		depthMap.bind(1);
		renderScene(*applyShadowShader);

		/*modelShader.enable();
		modelShader.setUniformMat4("model", model);
		modelShader.setUniform3f("viewPos", window.getCamPosition());
		modelShader.setUniform1i("skybox", 3);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
		nanosuit.Draw(modelShader);
		normalDisplayShader->enable();
		normalDisplayShader->setUniformMat4("model", model);
		nanosuit.Draw(*normalDisplayShader, false);*/

		glDepthFunc(GL_LEQUAL);
		skyboxShader->enable();
		skyboxShader->setUniformMat4("projection", projection);
		glm::mat4 cubeMapView = glm::mat4(glm::mat3(view));
		skyboxShader->setUniformMat4("view", cubeMapView);
		glBindVertexArray(skyboxVAO);
		cubeMap.bind();
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		float delta = timer.elapsed();
		window.update(delta);

		// FPS Timer block
		{
			frame++;
			if (t.cyclicTimer(1.0f))
			{
				printf("%d frame\n", frame);
				frame = 0;
			}
		}
	}

	return 0;
}
#endif


