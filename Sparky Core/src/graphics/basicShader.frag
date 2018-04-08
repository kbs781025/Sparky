#version 330 core
out vec4 FragColor;

in vec3 Position;
in vec3 Normal;

uniform samplerCube texture1;
uniform vec3 camPosition;

void main()
{
	vec3 viewDir = normalize(Position - camPosition);
	vec3 reflectDir = reflect(viewDir, normalize(Normal));
	FragColor = texture(texture1, reflectDir);
}
