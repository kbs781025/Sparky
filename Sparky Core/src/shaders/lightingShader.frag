#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	//sampler2D emission;
	float shininess;
};

struct Light
{
	vec3 position;
	vec3 direction;
	float cutOff;
	float outerCutOff;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	float constant;
	float linear;
	float quadratic;
};

in vec3 FragPosition;
in vec3 Normal;
in vec2 TexCoord;

uniform Light light;
uniform Material material;

void main()
{
	vec3 norm = normalize(Normal);
	//vec3 lightDir = normalize(light.position - FragPosition);
	vec3 fragmentToLight = light.position - FragPosition;
	vec3 lightDir = normalize(fragmentToLight);
	vec3 viewDir = normalize(-FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);

	float theta = dot(lightDir, -light.direction);
	float epsilon = light.cutOff - light.outerCutOff;
	float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);

	vec3 ambient = light.ambient * texture(material.diffuse, TexCoord).rgb;
	vec3 diffuse = max(dot(lightDir, norm), 0.0) * light.diffuse * texture(material.diffuse, TexCoord).rgb;
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * light.specular * texture(material.specular, TexCoord).rgb;

	//vec3 emission = texture(material.emission, TexCoord).rgb;

	float distance = length(fragmentToLight);
	float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));

	vec3 result = ambient * attenuation + (diffuse + specular) * attenuation * intensity;
	
	FragColor = vec4(result, 1.0);
}