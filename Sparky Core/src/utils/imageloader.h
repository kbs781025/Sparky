#pragma once

#include <string>
#include <stdio.h>
#include <FreeImage.h>

namespace sparky { namespace graphics {

	static BYTE* load_image(const char* filePath, GLsizei* width, GLsizei* height)
	{
		//image format
		FREE_IMAGE_FORMAT fif = FIF_UNKNOWN;
		//pointer to the image, once loaded
		FIBITMAP *dib = nullptr;

		//check the file signature and deduce its format
		fif = FreeImage_GetFileType(filePath, 0);
		//if still unknown, try to guess the file format from the file extension
		if (fif == FIF_UNKNOWN)
			fif = FreeImage_GetFIFFromFilename(filePath);
		//if still unkown, return failure
		if (fif == FIF_UNKNOWN)
			return nullptr;

		//check that the plugin has reading capabilities and load the file
		if (FreeImage_FIFSupportsReading(fif))
			dib = FreeImage_Load(fif, filePath);
		//if the image failed to load, return failure
		if (!dib)
			return nullptr;

		//retrieve the image data
		BYTE* result = FreeImage_GetBits(dib);
		//get the image width and height
		*width = FreeImage_GetWidth(dib);
		*height = FreeImage_GetHeight(dib);

		return result;
	}

}}