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
	vec4 worldLightPos;
};

out VS_DATA
{
	vec2 TexCoord;
	vec3 TangentFragPos;
	vec3 TangentViewDir;
	vec3 TangentLightDir;
} vs_out;

//layout (location = LIGHT_POSITION_LOCATION) uniform vec3 worldLightPos;

void main()
{
	gl_Position = MVP * vec4(aPos, 1.0);
	vs_out.TexCoord = aTexCoord;

	vec4 worldFragPos = model * vec4(aPos, 1.0);
	vec4 worldLightDir = worldLightPos - worldFragPos;
	vec4 worldViewDir = camPos - worldFragPos;

	vec3 T = normalize(mat3(model) * aTangent);
	vec3 B = normalize(mat3(model) * aBinormal);
	vec3 N = normalize(mat3(model) * aNormal);

	mat3 TBN = transpose(mat3(T, B, N));

	vs_out.TangentFragPos = TBN * vec3(model * vec4(aPos, 1.0));
	vs_out.TangentLightDir = TBN * worldLightDir.xyz;
	vs_out.TangentViewDir = TBN * worldViewDir.xyz;
}