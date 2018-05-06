#pragma once

#include <GL/glew.h>
#include <vector>
#include "Buffer.h"

namespace sparky { namespace graphics {

	class UniformBuffer : public Buffer
	{
	private:
		GLuint m_Handle;
		GLuint m_BlockSize;
	public:
		UniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize);
		UniformBuffer(const std::vector<float>& uniformBlockData, GLuint bindingPoint);
		~UniformBuffer();

		virtual void bind() const override;
		virtual void unBind() const override;

		virtual GLuint getHandle() const override;

		void setBindingPoint(GLuint bindingPoint);

		void setData(const std::vector<float>& uniformBlockData);
		void setDataSegment(GLuint uniformOffset, const std::vector<float>& uniformBlockData); 
		void setData(const void* data);
		void setDataSegment(GLuint uniformOffset, const void* data, GLuint byteSize);

		template<typename T>
		void setUniformBlockData(const std::vector<T>& blockData)
		{
			GLuint offset = 0;
			for (unsigned int i = 0; i < blockData.size(); i++)
			{
				setDataSegment(offset, &blockData[i], sizeof(T));
				offset += sizeof(T);
			}
		}
	private:
		void initUniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize);
		void initUniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize, const void* data);
	};

}}