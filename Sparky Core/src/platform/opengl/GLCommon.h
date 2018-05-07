#pragma once

#include <GL/glew.h>

namespace sparky
{
	extern GLenum GLCheckError();
	extern bool GLLogCall(const char* function, const char* file, int line);
}

#ifdef _DEBUG
#define GLCall(x) sparky::GLCheckError();\
		x; \
		if (!sparky::GLLogCall(#x, __FILE__, __LINE__)) __debugbreak();
#else
#define GLCall(x); x
#endif