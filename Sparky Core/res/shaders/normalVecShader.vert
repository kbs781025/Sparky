#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

out VS_OUT
{
	vec3 Normal;
}vs_out;

void main()
{
	gl_Position = projection * view * model  * vec4(aPos, 1.0);
	vs_out.Normal = normalize(projection * view * model * vec4(aNormal, 0.0)).rgb;
}
