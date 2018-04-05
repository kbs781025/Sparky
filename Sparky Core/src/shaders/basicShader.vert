#version 330 core

layout (location = 0) in vec3 avertexPos;
layout (location = 1) in vec2 atexCoord;

uniform mat4 MVP;
uniform mat4 projection;

out vec2 texCoord;

void main()
{
	texCoord = atexCoord;
	//gl_Position = projection * vec4(avertexPos, 1.0);
	gl_Position = MVP * vec4(avertexPos, 1.0);
}