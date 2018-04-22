#pragma once

#include <string>
#include <GL\glew.h>

namespace sparky {

	class ImageLoader
	{
	public:
		static unsigned char* load_Image(const char* filename, GLuint* width, GLuint* height, int* bits);

		static void free_image(unsigned char* data);
	};

}