layout(location = GBUFFER_POS) out vec3 WorldPosOut;
layout(location = GBUFFER_ALBEDO) out vec3 DiffuseOut;
layout(location = GBUFFER_NORMAL) out vec3 NormalOut;
layout(location = GBUFFER_TEXCOORD) out vec3 TexCoordOut;

in VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 Normals;
} fs_in;

layout (binding = DIFFUSE_TEXTURE_BINDING) uniform sampler2D texture_diffuse;

void main()
{
	WorldPosOut = fs_in.FragPos;
	DiffuseOut = texture(texture_diffuse, fs_in.TexCoord).xyz;
	NormalOut = normalize(fs_in.Normals);
	TexCoordOut = vec3(fs_in.TexCoord, 0.0);
}
