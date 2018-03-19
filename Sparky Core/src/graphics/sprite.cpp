#include "sprite.h"
#include "texture.h"

namespace sparky { namespace graphics {

	Sprite::Sprite(float x, float y, float width, float height, const glm::vec4 & color, Texture* texture)
		:
		Renderable2D(glm::vec3(x, y, 0), glm::vec2(width, height), color, texture)
	{
	}

}
}

