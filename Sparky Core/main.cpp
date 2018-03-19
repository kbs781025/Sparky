#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/texture.h"

#include <time.h>

static const float cubeVertex[] =
{
	-1.0f,-1.0f,-1.0f, // triangle 1 : begin
	-1.0f,-1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f, // triangle 1 : end
	1.0f, 1.0f,-1.0f, // triangle 2 : begin
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f, // triangle 2 : end
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	-1.0f,-1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f,-1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f,-1.0f,
	1.0f,-1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f,-1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f,-1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f,-1.0f, 1.0f
};

#if 0
int main()
{
	using namespace sparky;
	using namespace graphics;
	using namespace maths;

	Window window("Sparky", 800, 600);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

	Shader* shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	shader->enable();
	shader->setUniform2f("light_pos", vec2(4.0f, 1.5f));
	shader->setUniform1i("myTexture", 0);

	Texture* texture = new Texture("Texture/128/armor.png");

	TileLayer layer(shader);
	for (float y = -7.0f; y < 7.0f; y += 5)
	{
		for (float x = -14.f; x < 14.f; x += 5)
		{
			layer.add(new Sprite(x, y, 4.9f, 4.9f, vec4(0.3f, 0.6f, 1.0f, 1.0f), texture));
		}
	}
	
	glActiveTexture(GL_TEXTURE0);
	texture->bindTexture();

	Timer timer, t;
	unsigned int frame = 0;
	while (!window.closed())
	{
		timer.reset();

		window.clear();

		double x, y;
		window.getMouseCursorPosition(x, y);

		shader->enable();
		shader->setUniform2f("light_pos", vec2((float)(x * 32.0f / window.getWidth()- 16.0f), (float)(9.0f - y * 18.0f / window.getHeight())));

		//layer.render();

		window.update();

		frame++;
		if (t.elapsed() > 1.0f)
		{
			printf("%d frame\n", frame);
			frame = 0;
			t.reset();
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

	float* arrays[2] = { triangle1, triangle2 };

	unsigned int indicies[] =
	{
		0, 1, 3,
		1, 2, 3
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
	unsigned int VAORect, VBORect, EBORect;
	glGenVertexArrays(1, &VAORect);
	glGenBuffers(1, &VBORect);
	glGenBuffers(1, &EBORect);

	glBindVertexArray(VAORect);

	glBindBuffer(GL_ARRAY_BUFFER, VBORect);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBORect);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GL_FLOAT), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	Shader *pShader1 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	Shader *pShader2 = new Shader("src/shaders/basic.vert", "src/shaders/basic2.frag");

	Shader* pShaders[2] = { pShader1, pShader2 };

	// Generating a texture
	Texture *pTexture1 = new Texture("Texture/container.jpg");
	Texture *pTexture2 = new Texture("Texture/happyface.jpg");

	pShader1->enable();
	pShader1->setUniform1i("texture1", 0);
	pShader1->setUniform1i("texture2", 1);

	while (!window.closed())
	{
		timer.reset();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		window.clear();

		/*float timeValue = glfwGetTime();
		float greenValue = (sin(timeValue) / 2.0f) + 0.5f;
*/
		//for (int i = 0; i < 2; i++)
		//{
		//	pShaders[i]->enable();
		//	pShaders[i]->setUniform4f("ourColor", vec4(0.0f, greenValue, 0.0f, 1.0f));
		//	glBindVertexArray(VAO[i]);
		//	glDrawArrays(GL_TRIANGLES, 0, 3);
		//	//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		//}

		glActiveTexture(GL_TEXTURE0);
		pTexture1->bindTexture();
		glActiveTexture(GL_TEXTURE1);
		pTexture2->bindTexture();

		pShader1->setUniform1f("ratio", window.m_MixingRatio);
		glBindVertexArray(VBORect);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		
		window.update();

		frame++;
		if (t.elapsed() > 1.0f)
		{
			printf("%d frame\n", frame);
			frame = 0;
			t.reset();
		}
	}

	return 0;
}
#endif

