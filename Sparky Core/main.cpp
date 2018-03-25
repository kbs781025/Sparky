#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/texture.h"

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

#define NOSHOW
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

	std::vector<float> linePoints;
	linePoints.resize(6 * 6);
	setPoint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), linePoints, 0);
	setPoint(glm::vec3(2.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.0f, 0.0f), linePoints, 1);
	setPoint(glm::vec3(0.0f, 2.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), linePoints, 2);
	setPoint(glm::vec3(0.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 1.0f), linePoints, 3);
	setPoint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), linePoints, 4);
	setPoint(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 1.0f, 1.0f), linePoints, 5);

	unsigned int lineIndices[] =
	{
		0, 1,
		0, 2,
		0, 3,
		4, 5
	};

	unsigned int lineVAO, lineVBO, lineEBO;
	glGenVertexArrays(1, &lineVAO);
	glGenBuffers(1, &lineVBO);
	glGenBuffers(1, &lineEBO);

	glBindVertexArray(lineVAO);
	glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	glBufferData(GL_ARRAY_BUFFER, linePoints.size() * sizeof(GL_FLOAT), nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(0));
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, lineEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(lineIndices), lineIndices, GL_DYNAMIC_DRAW);

	Shader *pLightingShader = new Shader("src/shaders/lightingShader.vert", "src/shaders/lightingShader.frag");
	Shader *pLampShader = new Shader("src/shaders/lampShader.vert", "src/shaders/lampShader.frag");
	Shader *pLineShader = new Shader("src/shaders/lineShader.vert", "src/shaders/lineShader.frag");

	Texture *pDiffuseMap = new Texture("Texture/container2.png");
	Texture *pSpecularMap= new Texture("Texture/container2_specular.png");
	Texture *pEmissiomMap = new Texture("Texture/matrix.jpg");

	pLightingShader->enable();
	pLightingShader->setUniform1i("material.diffuse", 0);
	pLightingShader->setUniform1i("material.specular", 1); // if you use Uniform1f, you will get an error 1282.
	//pLightingShader->setUniform1i("material.emission", 2);
	
	glEnable(GL_DEPTH_TEST);
	float time = 0.0f;

	while (!window.closed())
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		window.clear();

		glm::mat4 view = window.getViewMatrix();
		glm::mat4 projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
		pLightingShader->enable();
		pLightingShader->setUniformMat4("view", view);
		pLightingShader->setUniformMat4("projection", projection);

		glm::vec3 lightPosition = glm::vec3(1.2f, 1.0f, 2.0f);
		//glm::vec3 lightPosition = window.getCamPosition();
		//glm::vec3 lightPositioninView = glm::vec3(view * glm::vec4(lightPosition, 1.0f)); // light position in view coordinates
		glm::vec3 lightPositioninView = glm::vec3(0.0f, 0.0f, 0.0f);
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		
		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = lightColor * glm::vec3(0.2f);
		glm::vec3 specularColor = glm::vec3(1.0f);

		#ifdef SHOW
		//moveLightPosition(lightPosition);
		//turnLightColor(lightColor);
		time += 0.01f;
		if (time > 1.0f)
			time = 0.0f;
		pLightingShader->setUniform1f("time", time);
		#endif

		pLightingShader->setUniform3f("light.position", lightPositioninView);
		pLightingShader->setUniform3f("light.direction", window.getForward());
		pLightingShader->setUniform1f("light.cutOff", glm::cos(glm::radians(12.5f)));
		pLightingShader->setUniform1f("light.outerCutOff", glm::cos(glm::radians(17.5f)));
		pLightingShader->setUniform3f("light.ambient", ambientColor);
		pLightingShader->setUniform3f("light.diffuse", diffuseColor);
		pLightingShader->setUniform3f("light.specular", specularColor);
		pLightingShader->setUniform1f("light.constant", 1.0f);
		pLightingShader->setUniform1f("light.linear", 0.09f);
		pLightingShader->setUniform1f("light.quadratic", 0.032f);

		pLightingShader->setUniform1f("material.shininess", 32.0f);

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

		glm::mat4 lightModel;
		lightModel = glm::translate(lightModel, lightPosition);
		lightModel = glm::scale(lightModel, glm::vec3(0.2f));

		pLampShader->enable();
		pLampShader->setUniformMat4("model", lightModel);
		pLampShader->setUniformMat4("view", view);
		pLampShader->setUniformMat4("projection", projection);

		glBindVertexArray(lightVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		pLineShader->enable();
		pLineShader->setUniformMat4("view", view);
		pLineShader->setUniformMat4("projection", projection);
		setPoint(window.getCamPosition() - glm::vec3(0.0f, 0.0f, 0.001f), glm::vec3(1.0f, 0.0f, 0.0f), linePoints, 4);
		setPoint(window.getForward() + window.getCamPosition(), glm::vec3(0.0f, 1.0f, 0.0f), linePoints, 5);
		
		glBindVertexArray(lineVAO);
		//glBufferSubData(GL_ARRAY_BUFFER, 4 * 6 * sizeof(GL_FLOAT), 2 * 6 * sizeof(GL_FLOAT), &linePoints[24]);
		glBufferSubData(GL_ARRAY_BUFFER, 0, linePoints.size() * sizeof(GL_FLOAT), linePoints.data());
		glLineWidth(5.0f);
		glDrawElements(GL_LINES, 8, GL_UNSIGNED_INT, 0);

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


