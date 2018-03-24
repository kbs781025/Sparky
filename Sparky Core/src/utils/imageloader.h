#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <FreeImage.h>
#include <iostream>
#include "stb_image.h"

namespace sparky {

	class ImageLoader
	{
	public:
		static unsigned char* load_Image(const char* filename, GLuint* width, GLuint* height, GLenum* format, bool tweak = false)
		{
			int nrComponents;
			unsigned char *data = stbi_load(filename, (int*)width, (int*)height, &nrComponents, 0);
			if (data)
			{
				switch (nrComponents)
				{
				case 1:
					*format = GL_RED;
					break;
				case 3:
					*format = GL_RGB;
					break;
				case 4:
					*format = GL_RGBA;
					break;
				}
			}
			else
			{
				std::cout << "Texture failed to load at path : " << filename << std::endl;
				stbi_image_free(data);
			}
			
			return data;
		}

		static void free_image(unsigned char* data)
		{
			stbi_image_free(data);
		}
	};

}