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
		UniformBuffer(GLuint uniformBlockSize, GLuint bindingPoint = 0);
		UniformBuffer(const std::vector<float>& uniformBlockData, GLuint bindingPoint = 0);
		~UniformBuffer();

		virtual void bind() const override;
		virtual void unBind() const override;

		virtual GLuint getHandle() const override;

		void setBindingPoint(GLuint bindingPoint);

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

		template<typename T>
		void setUniformBlockDataIndex(const T& blockData, GLuint index)
		{
			setDataSegment(sizeof(T) * index, &blockData, sizeof(T));
		}
	private:
		void setData(const std::vector<float>& uniformBlockData);
		void setDataSegment(GLuint uniformOffset, const std::vector<float>& uniformBlockData);
		void setData(const void* data);
		void setDataSegment(GLuint uniformOffset, const void* data, GLuint byteSize);
		void initUniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize);
		void initUniformBuffer(GLuint bindingPoint, GLuint uniformBlockSize, const void* data);
	};

}}