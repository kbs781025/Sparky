#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	sampler2D texture_normal0;
	float shininess;
};

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

in VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 lightPos;
	vec3 viewPos;
} fs_in;

#define NR_POINT_LIGHTS 5
layout(std140) uniform DirLights
{
	DirLight dirLight;
};

layout(std140) uniform PointLights
{
	PointLight pointLights[NR_POINT_LIGHTS];
};

uniform Material material;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcTangentPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = vec3(texture(material.texture_normal0, fs_in.TexCoord));
	normal = normalize(normal * 2.0 - 1.0);

	vec3 viewDir = normalize(fs_in.viewPos - fs_in.FragPos);

	vec3 result = vec3(0.0);
	for(int i=0; i<1; i++)
	{
		result += CalcTangentPointLight(pointLights[i], normal, fs_in.FragPos, viewDir);
	}
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal); 
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	vec3 specular = light.specular * specularIntensity * vec3(texture(material.texture_specular0, fs_in.TexCoord));

	return (ambient + diffuse + specular);
}

vec3  CalcTangentPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightColor = light.lightColor.rgb;
	vec3 lightDir = normalize(fs_in.lightPos - fragPos);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float dist = length(fs_in.lightPos - fragPos);
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

	vec3 result = (ambient + diffuse + specular) * attenuation;

	return pow(result, vec3(1/2.2));
}