#version 330 core

in vec3 FragColor;

out vec4 fragColor;

void main()
{
	fragColor = vec4(FragColor, 1.0);
}