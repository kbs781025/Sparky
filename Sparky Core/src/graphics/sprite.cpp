#include "sprite.h"
#include "texture.h"

namespace sparky { namespace graphics {

	Sprite::Sprite(float x, float y, float width, float height, const maths::vec4 & color, Texture* texture)
		:
		Renderable2D(maths::vec3(x, y, 0), maths::vec2(width, height), color, texture)
	{
	}

}
}

