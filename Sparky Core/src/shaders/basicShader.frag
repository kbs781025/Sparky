#version 330 core
out vec4 FragColor;

in vec3 Normal;
in vec3 Position;

uniform samplerCube texture1;
uniform vec3 camPosition;

void main()
{
	float ratio = 1.00 / 1.33;
	vec3 viewDir = normalize(Position - camPosition);
	vec3 reflectDir = reflect(viewDir, normalize(Normal));
	//vec3 reflectDir = refract(viewDir, normalize(Normal), ratio);
	FragColor = texture(texture1, reflectDir);
}
