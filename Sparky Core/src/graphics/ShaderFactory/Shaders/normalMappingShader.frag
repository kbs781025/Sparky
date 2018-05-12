#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse0;
	sampler2D texture_specular0;
	sampler2D texture_normal0;
	float shininess;
};

struct Light
{
	vec4 lightColor;
	vec4 position;
	vec4 direction;
	vec4 attenuation;
};

in VS_DATA
{
	vec3 FragPos;
	vec2 TexCoord;
	vec3 ViewPos;
	vec3 Normal;
	vec3 BiNormal;
	vec3 Tangent;
} fs_in;

#define NR_POINT_LIGHTS 1
layout(std140) uniform Lights
{
	Light lights[NR_POINT_LIGHTS];
};

uniform Material material;

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir);
vec3 CalcTangentPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = vec3(texture(material.texture_normal0, fs_in.TexCoord));
	mat3 normalToWorld = mat3(fs_in.Tangent, fs_in.BiNormal, fs_in.Normal);
	normal = normal * 2.0 - 1.0;
	normal *= normalToWorld;
	normal = normalize(normal);

	vec3 viewDir = normalize(fs_in.ViewPos - fs_in.FragPos);

	vec3 result = vec3(0.0);
	for(int i=0; i<1; i++)
	{
		result += CalcTangentPointLight(lights[i], normal, fs_in.FragPos, viewDir);
	}
	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(Light light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction.rgb);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal); 
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 lightColor = light.lightColor.rgb;
	vec3 ambient = lightColor * vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	vec3 diffuse = lightColor * diffuseIntensity * vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	vec3 specular = lightColor * specularIntensity * vec3(texture(material.texture_specular0, fs_in.TexCoord));

	return (ambient + diffuse + specular);
}

vec3  CalcTangentPointLight(Light light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightColor = light.lightColor.rgb;
	vec3 lightDir = normalize(light.position.rgb - fragPos);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float dist = length(light.position.rgb - fragPos);
	float attenuation = 1.0 / (light.attenuation.r + light.attenuation.g * dist + light.attenuation.b * dist * dist);

	vec3 albedo = vec3(texture(material.texture_diffuse0, fs_in.TexCoord));
	albedo = pow(albedo, vec3(2.2));
	vec3 ambient = lightColor * 0.2 * albedo;
	vec3 diffuse = lightColor * 0.7 * diffuseIntensity * albedo;
	vec3 specular = lightColor * 1.0 * specularIntensity;

	vec3 specularMap = vec3(texture(material.texture_specular0, fs_in.TexCoord));
	specular *= specularMap;

	vec3 result = (ambient + diffuse + specular) * attenuation;

	return pow(result, vec3(1/2.2));
}