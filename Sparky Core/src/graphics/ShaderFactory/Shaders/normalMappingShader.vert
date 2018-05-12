#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aNormal;
layout (location = 3) in vec3 aTangent;
layout (location = 4) in vec3 aBinormal;

layout (std140) uniform Matrices
{
	mat4 projection;
	mat4 view;
	mat4 model;
};

layout (std140) uniform Vectors
{
	vec4 cameraPos;
};

out VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 ViewPos;
	vec3 Normal;
	vec3 BiNormal;
	vec3 Tangent;
} vs_out;

uniform vec3 worldLightPos;

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;
	vs_out.ViewPos = cameraPos.rgb;
	vs_out.Normal = mat3(model) * aNormal;
	vs_out.BiNormal = mat3(model) * aBinormal;
	vs_out.Tangent = mat3(model) * aTangent;

	gl_Position = projection * view * model * vec4(aPos, 1.0);
}