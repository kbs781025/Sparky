#version 330 core
struct PointLight
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
	sampler2D texture_reflect1;
	float shininess;
};

in VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}fs_in;

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;
uniform samplerCube skybox;

out vec4 FragColor;

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir);

void main()
{
	vec3 result = vec3(0.0);
	vec3 norm = normalize(fs_in.Normal);
	vec3 viewDirection = normalize(viewPos - fs_in.FragPos);
	for(int i = 0; i < NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], norm, viewDirection);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fs_in.FragPos);
	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, fs_in.TexCoord));

	float diffuseIntensity = max(dot(lightDir, normal), 0.0);
	vec3 diffuse = diffuseIntensity * light.diffuse * vec3(texture(material.texture_diffuse1, fs_in.TexCoord));

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), 32);
	vec3 specular = light.specular * specularIntensity * vec3(texture(material.texture_specular1, fs_in.TexCoord));

	reflectDir = reflect(-viewDir, normal);
	vec3 reflColor = vec3(texture(skybox, reflectDir)); 
	vec3 reflOnModel = vec3(texture(material.texture_reflect1, fs_in.TexCoord));
	vec3 sumReflect = light.diffuse * reflColor * reflOnModel;

	float dist = length(light.position - fs_in.FragPos);
	float attenuation = 1.0 / (light.constant + dist * light.linear + dist * dist * light.quadratic);
	
	return (ambient + diffuse + specular + sumReflect) * attenuation;
	//return specular;
}