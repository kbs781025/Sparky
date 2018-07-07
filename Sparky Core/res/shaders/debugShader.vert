#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;

out DATA
{
	vec2 TexCoord;
} vs_out;

uniform mat4 MVP;

void main()
{
    gl_Position = MVP * vec4(aPos, 1.0);
	vs_out.TexCoord = aTexCoord;
}