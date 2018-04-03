#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/texture.h"
#include "src/graphics/model.h"

#include <time.h>
#include <vector>

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

#define NOSHOW
//#define MODEL_IMPORT
#ifndef MODEL_IMPORT
int main()
{
	using namespace sparky;
	using namespace graphics;

	Window window("Sparky", 800, 600);

	Timer timer, t;
	unsigned int frame = 0;

	float cubeVertex[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
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

	unsigned int cubeVAO, cubeVBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &cubeVBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), cubeVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	unsigned int planeVAO, planeVBO;
	glGenVertexArrays(1, &planeVAO);
	glGenBuffers(1, &planeVBO);

	glBindVertexArray(planeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);

	Shader depthTest = Shader("src/shaders/depthTest.vert", "src/shaders/depthTest.frag");
	Shader border = Shader("src/shaders/depthTest.vert", "src/shaders/lineShader.frag");
	Texture cubeTexture = Texture("Texture/container2.png");
	Texture planeTexture = Texture("Texture/floor.jpg");
	
	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		window.clear();

		glm::mat4 view = window.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		
		depthTest.enable();
		depthTest.setUniform1i("texture1", 0);

		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		glStencilMask(0x00);

		glm::mat4 model;
		glm::mat4 MVP = projection * view * model;
		depthTest.setUniformMat4("MVP", MVP);
		planeTexture.bindTexture();
		glBindVertexArray(planeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

		glStencilFunc(GL_ALWAYS, 1, 0xFF);
		glStencilMask(0xFF);

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			glm::mat4 MVP = projection * view * model;
			depthTest.setUniformMat4("MVP", MVP);
			glActiveTexture(GL_TEXTURE0);
			cubeTexture.bindTexture();
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
		}

		glDisable(GL_DEPTH_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilMask(0x00);
		border.enable();

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			model = glm::scale(model, glm::vec3(1.2f));
			glm::mat4 MVP = projection * view * model;
			depthTest.setUniformMat4("MVP", MVP);
			glActiveTexture(GL_TEXTURE0);
			cubeTexture.bindTexture();
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
		}

		glStencilMask(0xFF);
		glEnable(GL_DEPTH_TEST);

		#ifdef SHOW
		//moveLightPosition(lightPosition);
		//turnLightColor(lightColor);
		time += 0.01f;
		if (time > 1.0f)
			time = 0.0f;
		pLightingShader->setUniform1f("time", time);
		#endif
		
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

	Window window("Sparky", 800, 600);
	Model nanosuit("Texture/nanosuit/nanosuit.obj");
	Shader modelShader = Shader("src/shaders/modelShader.vert", "src/shaders/modelShader.frag");

	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(5.0f,  5.0f,  5.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f);

	modelShader.enable();
	for (int i = 0; i < 4; i++)
	{
		setPointLightUniforms(&modelShader, i, pointLightPositions[i],
							  pointLightColor * 0.2f , pointLightColor * 0.7f, pointLightColor * 1.0f,
							  1.0f, 0.007f, 0.0002f);
	}

	glEnable(GL_DEPTH_TEST);

	Timer timer, t;
	unsigned int frame = 0;

	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		window.clear();

		glm::mat4 model;
		model = glm::scale(model, glm::vec3(0.5f, 0.5f, 0.5f));
		glm::mat4 view = window.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		modelShader.setUniformMat4("model", model);
		modelShader.setUniformMat4("view", view);
		modelShader.setUniformMat4("projection", projection);
		modelShader.setUniform3f("viewPos", window.getCamPosition());

		nanosuit.Draw(modelShader);
	
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


