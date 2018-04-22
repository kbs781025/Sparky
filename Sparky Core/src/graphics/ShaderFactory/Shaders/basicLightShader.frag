#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D texture_diffuse1;
	sampler2D texture_specular1;
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
	vec3 position;

	float constant;
	float linear;
	float quadratic;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoord;

#define NR_POINT_LIGHTS 4
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform Material material;
uniform vec3 viewPos;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{
	vec3 normal = normalize(Normal);
	vec3 viewDir = normalize(viewPos - FragPosition);

	//vec3 result = CalcDirLight(dirLight, normal, viewDir);
	vec3 result = vec3(0.0);
	for(int i=0; i<NR_POINT_LIGHTS; i++)
	{
		result += CalcPointLight(pointLights[i], normal, FragPosition, viewDir);
	}

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
	vec3 lightDir = normalize(-light.direction);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal); 
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 specular = light.specular * specularIntensity * vec3(texture(material.texture_specular1, TexCoord));

	return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	vec3 lightDir = normalize(light.position - fragPos);
	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	vec3 reflectDir = reflect(-lightDir, normal);
	float specularIntensity = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

	float dist = length(light.position - fragPos);
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * dist * dist);

	vec3 ambient = light.ambient * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 diffuse = light.diffuse * diffuseIntensity * vec3(texture(material.texture_diffuse1, TexCoord));
	vec3 specular = light.specular * specularIntensity * vec3(texture(material.texture_specular1, TexCoord));

	return (ambient + diffuse + specular);
}
