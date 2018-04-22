#include "imageloader.h"
#include <iostream>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned char * sparky::ImageLoader::load_Image(const char * filename, GLuint * width, GLuint * height, int * bits)
{
	unsigned char *data = stbi_load(filename, (int*)width, (int*)height, bits, 0);
	if(data == nullptr)
	{
		std::cout << "Texture failed to load at path : " << filename << std::endl;
		stbi_image_free(data);
	}

	return data;
}

void sparky::ImageLoader::free_image(unsigned char * data)
{
	stbi_image_free(data);
}
