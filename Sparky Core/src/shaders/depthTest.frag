#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D texture1;

float near = 0.1;
float far = 100.0;

float calcDepth(float depthNDC)
{
	float depth = (2 * far * near) / ((far + near)  - depthNDC * (far - near));
	depth = (1.0 / (far - near)) * (-depth - near);
	return depth;
}

void main()
{
	FragColor = texture(texture1, TexCoord);
}
