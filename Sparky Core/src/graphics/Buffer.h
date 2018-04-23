#pragma once

#include <GL/glew.h>

namespace sparky { namespace graphics {

	class Buffer
	{
	public:
		virtual ~Buffer() {}
		virtual void bind() const = 0;
		virtual void unBind() const = 0;
		virtual GLuint getHandle() const = 0;
	};

}}