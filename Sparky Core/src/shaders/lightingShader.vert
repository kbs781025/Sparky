#version 330 core

layout (location = 0) in vec3 vertexPos;
layout (location = 1) in vec3 vertexNormal;

out vec3 FragColor;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPosition; 
uniform vec3 viewPosition;

void main()
{
	gl_Position = projection * view * model * vec4(vertexPos, 1.0);
	
	// Lighting in vertex shader(gouraud shading)
	vec3 fragPosition = vec3(model * vec4(vertexPos, 1.0));
	vec3 normal = normalize(vec3(transpose(inverse(model)) * vec4(vertexNormal, 1.0)));
	vec3 lightDir = normalize(lightPosition - fragPosition);
	vec3 viewDir = normalize(viewPosition - fragPosition);
	vec3 reflectDir = reflect(-lightDir, normal);

	float ambientIntensity = 0.2f;

	float diffuseIntensity = max(dot(lightDir, normal), 0.0);

	float specularIntensity = 1.0 * pow(max(dot(viewDir, reflectDir), 0.0), 2);

	FragColor = (ambientIntensity + diffuseIntensity + specularIntensity) * lightColor * objectColor;
}