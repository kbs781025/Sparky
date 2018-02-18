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

#include <time.h>

#include <FreeImage.h>

#if 0
int main()
{
	using namespace sparky;
	using namespace graphics;
	using namespace maths;

	Window window("Sparky", 800, 600);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	Shader* shader = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");
	Shader* shader2 = new Shader("src/shaders/basic.vert", "src/shaders/basic.frag");

	shader->enable();
	shader->setUniform2f("light_pos", vec2(4.0f, 1.5f));

	shader2->enable();
	shader2->setUniform2f("light_pos", vec2(0.0f, 0.0f));

	TileLayer layer(shader);
	for (float y = -7.0f; y < 7.0f; y += 0.3f)
	{
		for (float x = -14.f; x < 14.f; x += 0.3f)
		{
			layer.add(new Sprite(x, y, 0.2f, 0.2f, vec4(0.5f, 0.2f, 0.7f, 1.0f)));
		}
	}

	TileLayer layer2(shader2);
	layer2.add(new Sprite(0.0f, 0.0f, 5.f, 5.f, vec4(0.3f, 0.4f, 0.6f, 1.0f)));

	Group* button = new Group(mat4::translate(vec3(1,1,0)) * mat4::rotation(45.0f, vec3(0, 0, 1)));
	button->add(new Sprite(0.0f, 0.0f, 6.0f, 6.0f, vec4(0.5f, 0.2f, 0.1f, 1.0f)));
	button->add(new Sprite(2.0f, 2.f, 2.f, 2.f, vec4(0.4f, 0.6f, 0.1f, 1.0)));
	layer.add(button);

	Timer timer, t;
	unsigned int frame = 0;
	while (!window.closed())
	{
		timer.reset();

		window.clear();

		double x, y;
		window.getMouseCursorPosition(x, y);

		shader->enable();
		shader->setUniform2f("light_pos", vec2((float)(x * 32.0f / 960.0f - 16.0f), (float)(9.0f - y * 18.0f / 540.0f)));

		layer.render();

		//layer2.render();

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

int main()
{
	const char* filename = "tile.png";

	//image format
	FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
	//pointer to the image, once loaded
	FIBITMAP *dib(0);
	//pointer to the image data
	BYTE* bits(0);
	//image width and height
	unsigned int width(0), height(0);
	//OpenGL's image ID to map to

	//check the file signature and deduce its format
	fif = FreeImage_GetFileType(filename, 0);
	//if still unknown, try to guess the file format from the file extension
	if (fif == FIF_UNKNOWN)
		fif = FreeImage_GetFIFFromFilename(filename);
	//if still unkown, return failure
	if (fif == FIF_UNKNOWN)
		return false;

	//check that the plugin has reading capabilities and load the file
	if (FreeImage_FIFSupportsReading(fif))
		dib = FreeImage_Load(fif, filename);
	//if the image failed to load, return failure
	if (!dib)
		return false;

	//retrieve the image data
	bits = FreeImage_GetBits(dib);
	//get the image width and height
	width = FreeImage_GetWidth(dib);
	height = FreeImage_GetHeight(dib);
	//if this somehow one of these failed (they shouldn't), return failure
	if ((bits == 0) || (width == 0) || (height == 0))
		return false;

	for (int i = 0; i < width * height; i++)
	{
		std::cout << 
	}

	return 0;
}