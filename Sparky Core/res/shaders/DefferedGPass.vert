#version 330 core
layout (location = POSITION_ATTRIB_LOCATION) in vec3 aPos;
layout (location = TEXCOORD_ATTRIB_LOCATION) in vec2 aTexCoord;
layout (location = NORMAL_ATTRIB_LOCATION) in vec3 aNormal;
//layout (location = TANGENT_ATTRIB_LOCATION) in vec3 aTangent;
//layout (location = BINORMAL_ATTRIB_LOCATION) in vec3 aBinormal;

layout (std140, binding = MVP_UNIFORM_BLOCK_BINDING) uniform Matrices
{
	mat4 projection;
	mat4 view;
	mat4 model;
	mat4 MVP;
};

out VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	//mat3 TBN;
	vec3 Normal;
}vs_out;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	vs_out.FragPos = vec3(model * vec4(aPos, 1.0));
	vs_out.TexCoord = aTexCoord;
//	aNormal *= mat3(model);
//	aTangent *= mat3(model);
//	aBinormal *= mat3(model);
	vs_out.Normal = mat3(model) * aNormal;
}