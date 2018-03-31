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
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
	};

	unsigned int indicies[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glm::vec3 cubePositions[] =
	{
		glm::vec3(0.0f,  0.0f,  0.0f),
		glm::vec3(2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3(2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3(1.3f, -2.0f, -2.5f),
		glm::vec3(1.5f,  2.0f, -2.5f),
		glm::vec3(1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};

	glm::vec3 pointLightPositions[] =
	{
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	glm::vec3 pointLightColors[] = {
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.1f, 0.1f, 0.1f),
		glm::vec3(0.3f, 0.1f, 0.1f)
	};

	unsigned int cubeVAO, VBO;
	glGenVertexArrays(1, &cubeVAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(cubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), cubeVertex, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(2);

	unsigned int lightVAO;
	glGenVertexArrays(1, &lightVAO);

	glBindVertexArray(lightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	Shader *pLightingShader = new Shader("src/shaders/lightingShader.vert", "src/shaders/lightingShader.frag");
	Shader *pLampShader = new Shader("src/shaders/lampShader.vert", "src/shaders/lampShader.frag");
	Shader *pLineShader = new Shader("src/shaders/lineShader.vert", "src/shaders/lineShader.frag");

	Texture *pDiffuseMap = new Texture("Texture/container2.png");
	Texture *pSpecularMap= new Texture("Texture/container2_specular.png");
	Texture *pEmissiomMap = new Texture("Texture/matrix.jpg");

	// material uniform setting
	pLightingShader->enable();
	pLightingShader->setUniform1i("material.diffuse", 0);
	pLightingShader->setUniform1i("material.specular", 1); // if you use Uniform1f, you will get an error 1282.
	pLightingShader->setUniform1f("material.shininess", 32.0f);

	// pointlight uniform setting
	glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 diffuseColor = lightColor * glm::vec3(0.05f);
	glm::vec3 ambientColor = lightColor * glm::vec3(0.0f);
	glm::vec3 specularColor = glm::vec3(0.2f);

	for (int i = 0; i < 4; i++)
	{
		setPointLightUniforms(pLightingShader, i, pointLightPositions[i], pointLightColors[i] * 0.1f, pointLightColors[i], pointLightColors[i], 1.0f, 0.14f, 0.07f);
	}

	// Directional light from camera position uniform setting
	pLightingShader->setUniform3f("dirLight.direction", glm::vec3(-0.2f, -1.0f, -0.3f)); 
	pLightingShader->setUniform3f("dirLight.ambient", ambientColor);
	pLightingShader->setUniform3f("dirLight.diffuse", diffuseColor);
	pLightingShader->setUniform3f("dirLight.specular", glm::vec3(0.5f, 0.5f, 0.5f));

	glEnable(GL_DEPTH_TEST);
	
	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		window.clear();

		glm::mat4 view = window.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		pLightingShader->enable();
		pLightingShader->setUniformMat4("view", view);
		pLightingShader->setUniformMat4("projection", projection);
		pLightingShader->setUniform3f("viewPos", window.getCamPosition());

		#ifdef SHOW
		//moveLightPosition(lightPosition);
		//turnLightColor(lightColor);
		time += 0.01f;
		if (time > 1.0f)
			time = 0.0f;
		pLightingShader->setUniform1f("time", time);
		#endif

		glActiveTexture(GL_TEXTURE0);
		pDiffuseMap->bindTexture();
		glActiveTexture(GL_TEXTURE1);
		pSpecularMap->bindTexture();

		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			model = glm::translate(model, cubePositions[i]);
			float angle = i * 20;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			pLightingShader->enable();
			pLightingShader->setUniformMat4("model", model);
			glBindVertexArray(cubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}

		pLampShader->enable();
		pLampShader->setUniformMat4("view", view);
		pLampShader->setUniformMat4("projection", projection);
		for (int i = 0; i < 4; i++)
		{
			glm::mat4 lightModel;
			lightModel = glm::translate(lightModel, pointLightPositions[i]);
			lightModel = glm::scale(lightModel, glm::vec3(0.2f));
			pLampShader->setUniformMat4("model", lightModel);
			pLampShader->setUniform3f("lightColor", pointLightColors[i]);

			glBindVertexArray(lightVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
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


