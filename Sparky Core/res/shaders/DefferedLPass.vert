layout (location = POSITION_ATTRIB_LOCATION) in vec3 aPos;
layout (location = TEXCOORD_ATTRIB_LOCATION) in vec2 aTexCoord;

layout (std140, binding = MVP_UNIFORM_BLOCK_BINDING) uniform Matrices
{
	mat4 projection;
	mat4 view;
	mat4 model;
	mat4 MVP;
};

out vec2 TexCoord;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	TexCoord = aTexCoord;
}