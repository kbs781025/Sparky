#version 330 core
layout (location=0) in vec3 aPos;
layout (location=1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;
layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
};
uniform mat4 model;
uniform mat4 lightSpaceMatrix;

out VS_OUT
{
	vec4 posLight;
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0);
	vs_out.posLight = lightSpaceMatrix * model * vec4(aPos, 1.0);
	vs_out.fragPosition = vec3(model * vec4(aPos, 1.0));
	vs_out.normal = mat3(transpose(inverse(model))) * aNormal;
	vs_out.texCoord = aTexCoord;
}

