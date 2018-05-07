#version 330 core
#define SPECULAR_MAP
#define REFLECT_MAP
struct DirLight
{
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct PointLight
{
	vec4 lightColor;
	vec4 position;
	vec4 attenuation;
};

struct Material
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	#ifdef REFLECT_MAP
	sampler2D texture_reflect0;
	#endif
	float shininess;
};

in VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}fs_in;

#define NR_POINT_LIGHTS 1
layout(std140) uniform DirLights
{
	DirLight dirLight;
};

layout(std140) uniform PointLights
{
	PointLight pointLights[NR_POINT_LIGHTS];
};
uniform Material material;
uniform vec3 viewPos;

#ifdef REFLECT_MAP
uniform samplerCube skybox;
#endif

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);
vec3 CalcTangentPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 result = vec3(0.0);
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDirection = normalize(viewPos - fs_in.FragPos);
	for(int i = 0; i < 1; i++)
	{
		result += CalcPointLight(pointLights[i], norm, viewDirection);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightPos = light.position.rgb;
	vec3 lightColor = light.lightColor.rgb;
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float dist = length(lightPos - fs_in.FragPos);
	float attenuation = 1.0 / (light.attenuation.r + light.attenuation.g * dist + light.attenuation.b * dist * dist);

	vec3 albedo = vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	albedo = pow(albedo, vec3(2.2));
	vec3 ambient = lightColor * 0.2 * albedo;
	vec3 diffuse = lightColor * 0.7 * diffuseIntensity * albedo;
	vec3 specular = lightColor * 1.0 * specularIntensity;
	
	#ifdef SPECULAR_MAP
	vec3 specularMap = vec3(texture(material.texture_specular0, fs_in.TexCoord));
	specular *= specularMap;
	#endif
	
	vec3 reflectIntensity = vec3(0.0);

	#ifdef REFLECT_MAP
	vec3 skyboxAlbedo = vec3(texture(skybox, reflectDir)) * 0.2;
	vec3 reflectMap = vec3(texture(material.texture_reflect0, fs_in.TexCoord));
	reflectIntensity += skyboxAlbedo * reflectMap;
	#endif 

	vec3 result = (ambient + diffuse + specular + reflectIntensity) * attenuation;

	return pow(result, vec3(1/2.2));
}

vec3 CalcTangentPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightPos = light.position.rgb;
	vec3 lightColor = light.lightColor.rgb;
	vec3 lightDir = normalize(lightPos- fs_in.FragPos);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float dist = length(lightPos - fs_in.FragPos);
	float attenuation = 1.0 / (light.attenuation.r + light.attenuation.g * dist + light.attenuation.b * dist * dist);

	vec3 albedo = vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	albedo = pow(albedo, vec3(2.2));
	vec3 ambient = lightColor * 0.2 * albedo;
	vec3 diffuse = lightColor * 0.7 * diffuseIntensity * albedo;
	vec3 specular = lightColor * 1.0 * specularIntensity;
	vec3 specularMap = vec3(texture(material.texture_specular0, fs_in.TexCoord));
	if(specularMap.r > 0.0)
	{
		specular *= specularMap;
	}

	vec3 result = (ambient + diffuse + specular) * attenuation;

	return pow(result, vec3(1/2.2));
}
