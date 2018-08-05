#pragma once

#include <GL/glew.h>

namespace sparky { namespace graphics {

	class ShadowMapFBO
	{
	public:
		ShadowMapFBO();

		~ShadowMapFBO();

		bool Init(unsigned int WindowWidth, unsigned int WindowHeight);

		void BindForWriting();

		void BindForReading(GLenum TextureUnit);

	private:
		GLuint m_FBO;
		GLuint m_ShadowMap;
	};

}}