layout (location = POSITION_ATTRIB_LOCATION) in vec3 aPos;
layout (location = TEXCOORD_ATTRIB_LOCATION) in vec2 aTexCoord;

out vec2 TexCoords;

void main()
{
	TexCoords = aTexCoord;
	gl_Position = vec4(aPos, 1.0);
}