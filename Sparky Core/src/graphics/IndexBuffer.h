#pragma once

#include <GL/glew.h>
#include <vector>
#include "Buffer.h"

namespace sparky { namespace graphics {

	struct IndexBufferContext
	{
		const void* IBData;
		GLuint Count;

		IndexBufferContext(const void* IBarray, GLuint count)
		{
			IBData = IBarray;
			Count = count;
		}

		IndexBufferContext(const std::vector<float>& IBvector)
		{
			IBData = IBvector.data();
			Count = IBvector.size();
		}
	};

	class IndexBuffer : public Buffer
	{
	private:
		GLuint m_Handle;
		GLuint m_Count; // number of index(not in bytes)
	public:
		IndexBuffer(const void* data, GLuint count);
		IndexBuffer(const IndexBufferContext& context);
		~IndexBuffer();

		virtual void bind() const override;
		virtual void unBind() const override;

		virtual GLuint getHandle() const override;
		inline GLuint getCount() const { return m_Count; }
	private:
		void initIBObject(const void* data);
	};

}}