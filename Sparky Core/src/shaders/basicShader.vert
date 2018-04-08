#version 330 core

layout (location = 0) in vec3 avertexPos;
layout (location = 1) in vec3 aNormal;
layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};

uniform mat4 model;

out vec3 Normal;
out vec3 Position;

void main()
{
	Position = vec3(model * vec4(avertexPos, 1.0));
	Normal = mat3(transpose(inverse(model))) * aNormal;
	gl_Position = projection * view * model * vec4(avertexPos, 1.0);
}