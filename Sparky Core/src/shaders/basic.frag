#version 330 core

layout (location = 0) out vec4  color;

uniform vec2 light_pos;
uniform sampler2D myTexture;

in DATA
{
	vec4 position;
	vec2 uv;
	vec4 color;
} fs_in;

void main()
{
	float intensity = 1.0 / length(fs_in.position.xy - light_pos);
	color = texture(myTexture, fs_in.uv);
	color = color * intensity;
}