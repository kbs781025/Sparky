#version 330 core

in vec3 FragPosition;
in vec3 Normal;

out vec4 fragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition;

void main()
{
	vec3 norm = normalize(Normal);
	vec3 lightDir = normalize(lightPosition - FragPosition);

	float ambientIntensity = 0.2;
	float diffuseIntensity = max(dot(norm, lightDir), 0.0);
	
	vec3 viewDir = normalize(-FragPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float specularIntensity = 0.5 * pow(max(dot(reflectDir, viewDir), 0.0), 32);

	vec3 result = (ambientIntensity + diffuseIntensity + specularIntensity) * lightColor * objectColor;
	fragColor = vec4(result, 1.0);
}