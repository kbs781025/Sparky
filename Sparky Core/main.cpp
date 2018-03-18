#include "src/utils/timer.h"
#include "src/graphics/window.h"
#include "src/maths/maths.h"
#include "src/graphics/shaders.h"
#include "src/graphics/buffers/vertexarray.h"
#include "src/graphics/buffers/indexbuffer.h"

#include "src/graphics/renderable2d.h"
#include "src/graphics/simple2drenderer.h"
#include "src/graphics/staticsprite.h"
#include "src/graphics/sprite.h"
#include "src/graphics/batchrenderer2d.h"
#include "src/graphics/layer/tilelayer.h"
#include "src/graphics//layer/group.h"
#include "src/graphics/texture.h"

#include <time.h>

#include <FreeImage.h>

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
	using namespace maths;

	Window window("Sparky", 800, 600);

	Timer timer, t;
	unsigned int frame = 0;

	float vertices[] =
	{
		0.5f,  0.5f, 0.0f,  // top right
		0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f
	};

	float triangle1[] =
	{
		-0.5f, 0.5f, 0.0f,
		-1.0f, -0.5f, 0.0f,
		0.0f, -0.5f, 0.0f
	};

	float triangle2[] =
	{
		0.0f, 0.5f, 0.0f,
		1.0f, 0.5f, 0.0f,
		0.5f, -0.5f, 0.0f
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
		glBufferData(GL_ARRAY_BUFFER, 36, arrays[i], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
		glEnableVertexAttribArray(0);
	}

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	/*unsigned int VAO, VBO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(triangleVertices), triangleVertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicies), indicies, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);*/

	Shader *pShader1 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	Shader *pShader2 = new Shader("src/shaders/basic.vert", "src/shaders/basic2.frag");

	Shader* pShaders[2] = { pShader1, pShader2 };

	while (!window.closed())
	{
		timer.reset();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		window.clear();

		for (int i = 0; i < 2; i++)
		{
			pShaders[i]->enable();
			glBindVertexArray(VAO[i]);
			glDrawArrays(GL_TRIANGLES, 0, 3);
			//glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
		}
		
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

