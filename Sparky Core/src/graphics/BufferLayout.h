#pragma once

#include <GL/glew.h>
#include <vector>
#include <string>
#include "../maths/maths.h"

namespace sparky { namespace graphics {

	struct BufferElement
	{
		std::string Name;
		GLuint Count;
		GLenum Type;
		GLuint Size;
		bool Normalize;

		BufferElement(const std::string& name, GLuint count, GLenum type, bool normalize)
			: Name(name), Count(count), Type(type), Normalize(normalize)
		{
			Size = Count * sizeof(Type);
		}
	};

	class BufferLayout
	{
	private:
		GLuint m_Size;
		std::vector<BufferElement> m_Elements;
	public:
		BufferLayout();
		BufferLayout(std::vector<BufferElement>& elements);

		template<typename T>
		void Push(const std::string& name, bool normalize)
		{
			return;
		}

		template <>
		void Push<glm::vec3>(const std::string& name, bool normalize)
		{
			Push(name, 3, GL_FLOAT, normalize);
		}

		template<>
		void Push<glm::vec2>(const std::string& name, bool normalize)
		{
			Push(name, 2, GL_FLOAT, normalize);
		}

		void PushPosition();
		void PushTexCoord();
		void PushTangent();
		void PushBinormal();
		void PushNormal();

		inline const std::vector<BufferElement>& getLayOut() const { return m_Elements; }
		inline GLuint getStride() const { return m_Size; }
	private:
		void Push(const std::string& name, GLuint count, GLenum type, bool normalize);
	public:
		static BufferLayout getPosNormTexLayout();
		static BufferLayout getPosTexLayout();
		static BufferLayout getPosLayout();
		static BufferLayout getPosTexTBNLayout();
	};

}}