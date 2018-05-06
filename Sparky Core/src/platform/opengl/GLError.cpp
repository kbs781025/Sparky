#include "GLCommon.h"
#include <iostream>

namespace sparky
{
	GLenum GLCheckError()
	{
		return glGetError();
	}

	bool GLLogCall(const char* function, const char* file, int line)
	{
		GLenum error = GLCheckError();
		if (error != GL_NO_ERROR)
		{
			//SP_ERROR("[OpenGL Error] (", error, "): ", function, " ", file, ":", line);
			printf("[OpenGL Error] ( %d ):, %s, %s, : %d", error, function, file, line);
			return false;
		}
		return true;
	}
}