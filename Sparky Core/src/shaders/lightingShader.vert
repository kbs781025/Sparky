#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;
layout (location = 2) in vec2 texCoord;

out vec3 FragPosition;
out vec3 Normal;
out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPos, 1.0);
	FragPosition = vec3(view * model * vec4(vertexPos, 1.0));
	Normal = mat3(transpose(inverse(view * model))) * vertexNormal;
	TexCoord = texCoord;
}