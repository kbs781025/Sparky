layout (location = POSITION_ATTRIB_LOCATION) in vec3 aPos;
layout (location = TEXCOORD_ATTRIB_LOCATION) in vec2 aTexCoord;
layout (location = NORMAL_ATTRIB_LOCATION) in vec3 aNormal;
layout (location = TANGENT_ATTRIB_LOCATION) in vec3 aTangent;
layout (location = BINORMAL_ATTRIB_LOCATION) in vec3 aBinormal;

layout (std140, binding = MVP_UNIFORM_BLOCK_BINDING) uniform Matrices
{
	mat4 projection;
	mat4 view;
	mat4 model;
	mat4 MVP;
};

layout (std140, binding = VEC_UNIFORM_BLOCK_BINDING) uniform Vectors
{
	vec4 camPos;
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
	vs_out.ViewPos = camPos.rgb;
	vs_out.Normal = mat3(model) * aNormal;
	vs_out.BiNormal = mat3(model) * aBinormal;
	vs_out.Tangent = mat3(model) * aTangent;

	gl_Position = MVP * vec4(aPos, 1.0);
}