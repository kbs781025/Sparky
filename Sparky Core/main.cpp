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

#if 1
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
		shader->setUniform2f("light_pos", vec2((float)(x * 32.0f / 800.0f- 16.0f), (float)(9.0f - y * 18.0f / 600.0f)));

		layer.render();

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
