#version 330 core
out vec4 FragColor;

struct Material
{
	sampler2D diffuse;
	sampler2D specular;
	sampler2D emission;
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
in vec2 TexCoord;

uniform vec3 lightPosition;
uniform Light light;
uniform Material material;
uniform float time;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(light.position - FragPosition);
	vec3 viewDir = normalize(-FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);

	vec3 ambient = light.ambient * texture(material.specular, TexCoord).rgb;
	vec3 diffuse = max(dot(lightDir, norm), 0.0) * light.diffuse * texture(material.emission, TexCoord).rgb;
	vec3 specular = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess) * light.specular * texture(material.specular, TexCoord).rgb;

	//vec3 emission = texture(material.emission, TexCoord).rgb;
	vec3 emission = vec3(0.0);
	if(texture(material.specular, TexCoord).r == 0.0)
	{
		emission = texture(material.emission, TexCoord).rgb;
		//emission = texture(material.emission, TexCoord + vec2(0.0, time)).rgb;
	}

	vec3 result = ambient + diffuse + specular + emission;
	FragColor = vec4(result, 1.0);
}