#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTexCoord;
layout (location = 2) in vec3 aTangent;
layout (location = 3) in vec3 aBinormal;
layout (location = 4) in vec3 aNormal;
layout (std140) uniform Matrices
{
	mat4 MVP;
};

uniform mat4 model;

//out VS_DATA
//{
//	vec3 FragPos;
//	vec2 TexCoord;
//	mat3 TBN;
//} vs_out;

out VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 lightPos;
	vec3 viewPos;
} vs_out;

uniform vec3 worldLightPos;
uniform vec3 worldViewPos;

void main()
{
	vec3 FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;
	vec3 T = normalize(mat3(model) * aTangent);
	vec3 B = normalize(mat3(model) * aBinormal);
	vec3 N = normalize(mat3(model) * aNormal);
	mat3 TBN = mat3(T, B, N);
	TBN = transpose(TBN);

	vs_out.FragPos = TBN * FragPos;
	vs_out.lightPos = TBN * worldLightPos;
	vs_out.viewPos = TBN * worldViewPos;

	gl_Position = MVP * vec4(aPos, 1.0);
}