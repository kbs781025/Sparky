#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/texture.h"

#include <time.h>


int main()
{
	using namespace sparky;
	using namespace graphics;

	Window window("Sparky", 800, 600);

	Timer timer, t;
	unsigned int frame = 0;

	float vertices[] =
	{
		0.5f,  0.5f, 0.0f,		1.0f, 0.0f, 0.0f,		2.0f, 2.0f, // top right
		0.5f, -0.5f, 0.0f,		0.0f, 1.0f, 0.0f,		2.0f, 0.0f,// bottom right
		-0.5f, -0.5f, 0.0f,		0.0f, 0.0f, 1.0f,		0.0f, 0.0f, // bottom left
		-0.5f,  0.5f, 0.0f,		1.0f, 1.0f, 0.0f,		0.0f, 2.0f // top left
	};
	
	float triangle1[] =
	{
		-0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		-1.0f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

	float triangle2[] =
	{
		0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f,
		1.0f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f,
		0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f
	};

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

	float* arrays[2] = { triangle1, triangle2 };

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

	// VAO & VBO for triangle 1
	unsigned int VAO[2], VBO[2];
	glGenVertexArrays(2, VAO);
	glGenBuffers(2, VBO);

	for (int i = 0; i < 2; i++)
	{
		glBindVertexArray(VAO[i]);

		glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
		glBufferData(GL_ARRAY_BUFFER, 6 * 3 * sizeof(GL_FLOAT), arrays[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(1);
	}

	// VAO & VBO & EBO for rectangle
	unsigned int VAORect, VBORect/*, EBORect*/;
	glGenVertexArrays(1, &VAORect);
	glGenBuffers(1, &VBORect);
	//glGenBuffers(1, &EBORect);

	glBindVertexArray(VAORect);

	glBindBuffer(GL_ARRAY_BUFFER, VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertex), cubeVertex, GL_STATIC_DRAW);

	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORect);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	Shader *pShader1 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");

	// Generating a texture
	Texture *pTexture1 = new Texture("Texture/container.jpg");
	Texture *pTexture2 = new Texture("Texture/happyface.jpg");

	glActiveTexture(GL_TEXTURE0);
	pTexture1->bindTexture();
	glActiveTexture(GL_TEXTURE1);
	pTexture2->bindTexture();

	pShader1->enable();
	pShader1->setUniform1i("texture1", 0);
	pShader1->setUniform1i("texture2", 1);

	glEnable(GL_DEPTH_TEST);

	while (!window.closed())
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		window.clear();

		pShader1->setUniform1f("ratio", window.m_MixingRatio);

		glBindVertexArray(VBORect);
		for (int i = 0; i < 10; i++)
		{
			glm::mat4 model;
			float angle = 20.0f * i;
			model = glm::translate(model, cubePositions[i]);

			if (i % 3 == 0)
			{
				angle = (float)glfwGetTime() * 20.0f;
			}
			model = glm::rotate(model, glm::radians(angle), glm::vec3(0.5f, 1.0f, 0.3f));
			
			glm::mat4 view;
			view = window.getViewMatrix();

			glm::mat4 projection;
			projection = glm::perspective(glm::radians(window.getFov()), (float)window.getWidth() / window.getHeight(), 0.1f, 100.0f);
			pShader1->setUniformMat4("model", model);
			pShader1->setUniformMat4("view", view);
			pShader1->setUniformMat4("projection", projection);

			glDrawArrays(GL_TRIANGLES, 0, 36); // draw 36 vertices without index buffer
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


