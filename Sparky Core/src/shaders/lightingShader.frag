#version 330 core
struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float shininess;
};

struct Light
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 position;
};

in vec3 FragPosition;
in vec3 Normal;

out vec4 fragColor;

uniform vec3 lightPosition;
uniform Light light;
uniform Material material;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	vec3 viewDir = normalize(-FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = light.ambient * material.ambient;
	vec3 diffuse = max(dot(lightDir, norm), 0.0) * light.diffuse * material.diffuse;
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * light.specular * material.specular;

	vec3 result = ambient + diffuse + specular;
	fragColor = vec4(result, 1.0);
}