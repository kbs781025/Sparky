layout(location = GBUFFER_POS) out vec3 GBPosition;
layout(location = GBUFFER_NORMAL) out vec3 GBNormal;
layout(location = GBUFFER_ALBEDO_SPECULAR) out vec4 GBAlbedo_Specular;

in VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	mat3 TBN;
} fs_in;

struct Light
{
	vec4 lightColor;
	vec4 position;
	vec4 direction;
	vec4 attenuation;
};

layout (binding = DIFFUSE_TEXTURE_BINDING) uniform sampler2D texture_diffuse;
layout (binding = SPEUCLAR_TEXTURE_BINDING) uniform sampler2D texture_specular;
layout (binding = NORMAL_TEXTURE_BINDING) uniform sampler2D texture_normal;
layout (location = SPECULAR_SHININESS_LOCATION) uniform float shininess;

void main()
{
	GBPosition = fs_in.FragPos;
	vec3 normal = texture(texture_normal, fs_in.TexCoord).rgb * 2.0 - 1.0;
	normal = fs_in.TBN * normal;
	GBNormal = normal;
	
	GBAlbedo_Specular.rgb = texture(texture_diffuse, fs_in.TexCoord).rgb;
	GBAlbedo_Specular.a = texture(texture_specular, fs_in.TexCoord).a;
}
