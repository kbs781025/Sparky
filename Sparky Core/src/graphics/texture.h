#pragma once

#include <GL/glew.h>
#include <string>

namespace sparky { namespace graphics {

	enum class TextureWrap
	{
		NONE = 0,
		REPEAT,
		CLAMP,
		MIRRORED_REPEAT,
		CLAMP_TO_EDGE,
		CLAMP_TO_BORDER
	};

	enum class TextureFilter
	{
		NONE = 0,
		LINEAR,
		NEARES
	};

	enum class TextureFormat
	{
		NONE = 0,
		RGB,
		RGBA,
		LUMINANCE,
		LUMINANCE_ALPHA
	};

	struct TextureParameters
	{
		TextureFormat format;
		TextureFilter filter;
		TextureWrap wrap;

		TextureParameters()
		{
			format = TextureFormat::RGBA;
			filter = TextureFilter::LINEAR;
			wrap = TextureWrap::REPEAT;
		}

		TextureParameters(TextureFormat format, TextureFilter filter, TextureWrap wrap)
			: format(format), filter(filter), wrap(wrap)
		{
		}

		TextureParameters(TextureFilter filter)
			: format(TextureFormat::RGBA), filter(filter), wrap(TextureWrap::CLAMP)
		{
		}

		TextureParameters(TextureFilter filter, TextureWrap wrap)
			: format(TextureFormat::RGBA), filter(filter), wrap(wrap)
		{
		}
	};

	struct TextureLoadOptions
	{
		bool flipX;
		bool flipY;

		TextureLoadOptions()
			: flipX(false), flipY(false)
		{
		}

		TextureLoadOptions(bool flipX, bool flipY)
			: flipX(flipX), flipY(flipY)
		{
		}
	};

	class Texture
	{
	public:
		virtual ~Texture() {}

		virtual void bind(GLuint slot = 0) const = 0;
		virtual void unBind(GLuint slot = 0) const = 0;

		virtual const std::string& getFilePath() const = 0;
		virtual GLuint getHandle() const = 0;

		virtual GLuint getWidth() const = 0;
		virtual GLuint getHeight() const = 0;
	};

}}