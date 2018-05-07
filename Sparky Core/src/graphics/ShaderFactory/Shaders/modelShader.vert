#version 330 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;
layout (std140) uniform Matrices
{
	mat4 MVP;
};
uniform mat4 model;

out VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}vs_out;

void main()
{
	vs_out.Normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.FragPos = vec3(model * vec4(aPosition, 1.0));
	vs_out.TexCoord = aTexCoord;

	gl_Position = MVP * vec4(aPosition, 1.0);
}