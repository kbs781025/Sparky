#pragma once

#define STB_IMAGE_IMPLEMENTATION

#include <string>
#include <iostream>
#include "stb_image.h"

namespace sparky {

	class ImageLoader
	{
	public:
		static unsigned char* load_Image(const char* filename, GLuint* width, GLuint* height, GLenum* internalFormat, GLenum* dataFormat)
		{
			int nrComponents;
			unsigned char *data = stbi_load(filename, (int*)width, (int*)height, &nrComponents, 0);
			if (data)
			{
				switch (nrComponents)
				{
				case 1:
					*internalFormat = GL_RED;
					*dataFormat = GL_RED;
					break;
				case 3:
					*internalFormat = GL_SRGB;
					*dataFormat = GL_RGB;
					break;
				case 4:
					*internalFormat = GL_SRGB_ALPHA;
					*dataFormat = GL_RGBA;
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