#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/texture.h"
#include "src/graphics/model.h"

#include <time.h>
#include <vector>
#include <map>

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
		/*-0.5f, 0.5f, 0.0f, 0.0f, 0.0f,
		-0.5f, -0.5f, 0.0f, 0.0f, 1.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		0.5f, 0.5f, 0.0f, 1.0f, 0.0f,
		-0.5, 0.5, 0.0, 0.0f, 1.0f,
		0.5, -0.5, 0.0, 1.0f, 1.0f*/
		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		0.0f, -0.5f,  0.0f,  0.0f,  1.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,

		0.0f,  0.5f,  0.0f,  0.0f,  0.0f,
		1.0f, -0.5f,  0.0f,  1.0f,  1.0f,
		1.0f,  0.5f,  0.0f,  1.0f,  0.0f
	};

	float screenWidth = window.getWidth();
	float screenHeight = window.getHeight();
	float screenVertex1[] =
	{
		/*0.0f, screenHeight / 2.0f, 5.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 0.5f, 0.0f, 0.0f,
		screenWidth / 2.0f, screenHeight / 2.0f, 5.0f, 1.0f, 1.0f,
		screenWidth / 2.0f, screenHeight / 2.0f, 5.0f, 1.0f, 1.0f,
		0.0f, 0.0f, 5.0f, 0.0f, 0.0f,
		screenWidth / 2.0f, 0.0f, 5.0f, 1.0f, 0.0f*/
		-1.0f,  1.0f,  5.0f, 0.0f, 1.0f,
		-1.0f, -1.0f,  5.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  5.0f, 1.0f, 0.0f,

		-1.0f,  1.0f,  5.0f, 0.0f, 1.0f,
		1.0f, -1.0f,  5.0f, 1.0f, 0.0f,
		1.0f,  1.0f,  5.0f, 1.0f, 1.0f
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

	std::vector<glm::vec3> grassPositions;
	grassPositions.push_back(glm::vec3(-1.5f, 0.0f, -0.48f));
	grassPositions.push_back(glm::vec3(1.5f, 0.0f, 0.51f));
	grassPositions.push_back(glm::vec3(0.0f, 0.0f, 0.7f));
	grassPositions.push_back(glm::vec3(-0.3f, 0.0f, -2.3f));

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

	unsigned int grassVAO, grassVBO;
	glGenVertexArrays(1, &grassVAO);
	glGenBuffers(1, &grassVBO);

	glBindVertexArray(grassVAO);
	glBindBuffer(GL_ARRAY_BUFFER, grassVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertex), quadVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	unsigned int screenVAO, screenVBO;
	glGenVertexArrays(1, &screenVAO);
	glGenBuffers(1, &screenVBO);

	glBindVertexArray(screenVAO);
	glBindBuffer(GL_ARRAY_BUFFER, screenVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenVertex1), screenVertex1, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	
	glBindVertexArray(0);

	unsigned int framebuffer;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	Texture framebufferTexture = Texture(window.getWidth(), window.getHeight());
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, framebufferTexture.getTextureID(), 0);

	unsigned int rbo;
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, window.getWidth(), window.getHeight());
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		std::cout << "Error::FRAMEBUFFER:: Framebuffer is not complete" << std::endl;
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	Shader blend = Shader("src/shaders/blendShader.vert", "src/shaders/blendShader.frag");
	Shader basicShader = Shader("src/shaders/basicShader.vert", "src/shaders/basicShader.frag");
	Texture planeTexture = Texture("Texture/floor.jpg");
	Texture cubeTexture = Texture("Texture/container2.png");
	Texture grassTexture = Texture("Texture/grass.png");
	Texture windowTexture = Texture("Texture/blending_transparent_window.png");
	
	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		window.clear();

		glm::mat4 view = window.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		glm::mat4 ortho = glm::ortho(window.getWidth() / -2.0f, window.getWidth() / 2.0f, window.getHeight() / -2.0f, window.getHeight() / 2.0f
									, 0.0f, 100.0f);
		
		blend.enable();
		blend.setUniform1i("texture1", 0);

		glm::mat4 model;
		glm::mat4 MVP = projection * view * model;
		blend.setUniformMat4("MVP", MVP);
		glActiveTexture(GL_TEXTURE0);
		planeTexture.bindTexture();
		glBindVertexArray(planeVAO);

		glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
		glEnable(GL_DEPTH_TEST);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES, 0, 3 * 2);

		for (int i = 0; i < 4; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			glm::mat4 MVP = projection * view * model;
			blend.setUniformMat4("MVP", MVP);
			glActiveTexture(GL_TEXTURE0);
			cubeTexture.bindTexture();
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6 * 6);
		}

		std::map<float, glm::vec3> windowSortedInDist;
		for (unsigned int i = 0; i < grassPositions.size(); i++)
		{
			float dist = glm::distance(grassPositions[i], window.getCamPosition());
			windowSortedInDist[dist] = grassPositions[i];
		}
		
		for (std::map<float, glm::vec3>::reverse_iterator rit = windowSortedInDist.rbegin(); rit != windowSortedInDist.rend(); ++rit)
		{
			glm::mat4 model;
			model = glm::translate(model, rit->second);
			glm::mat4 MVP = projection * view * model;
			blend.setUniformMat4("MVP", MVP);
			glActiveTexture(GL_TEXTURE0);
			windowTexture.bindTexture();
			glBindVertexArray(grassVAO);
			glDrawArrays(GL_TRIANGLES, 0, 1 * 6);
		}

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(screenVAO);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
		framebufferTexture.bindTexture();
		basicShader.enable();
		basicShader.setUniformMat4("projection", projection); // z needs to be negative
		basicShader.setUniform1i("texture1", 0);
		glDrawArrays(GL_TRIANGLES, 0, 6);

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


