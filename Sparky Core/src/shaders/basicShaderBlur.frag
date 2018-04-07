#version 330 core
out vec4 fragColor;

in vec2 texCoord;

uniform sampler2D texture1;
const float offset = 1.0 / 300.0;

void main()
{
	vec2 offsets[9] = vec2[](
		vec2(-offset, offset),
		vec2(0.0, offset),
		vec2(offset, offset),
		vec2(-offset, 0.0),
		vec2(0.0, 0.0),
		vec2(offset, 0.0),
		vec2(-offset, -offset),
		vec2(0, -offset),
		vec2(offset, -offset)
	);

	float kernel[9] = float[](
		2.0 / 32, 4.0 / 32, 2.0 / 32,
		4.0 / 32, 8.0 / 32, 4.0 / 32,
		2.0 / 32, 4.0 / 32, 2.0 / 32
	);

	vec3 sampleTex[9];
	for(int i=0; i<9; i++)
	{
		sampleTex[i] = vec3(texture(texture1, texCoord + offsets[i]));
	}

	vec3 color = vec3(0.0);
	for(int i=0; i<9; i++)
	{
		color += sampleTex[i] * kernel[i];
	}

	fragColor = vec4(color, 1.0);
}