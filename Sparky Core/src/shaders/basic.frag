#version 330 core

layout (location = 0) out vec4  color;

in DATA
{
	vec4 position;
	vec2 uv;
	float textureID;
	vec4 color;
} fs_in;

uniform vec2 light_pos;
uniform sampler2D textures[32];

void main()
{
	float intensity = 1.0 / length(fs_in.position.xy - light_pos);

	vec4 texColor = fs_in.color;

	if(fs_in.textureID > 0.0)
	{
		int tid = int(fs_in.textureID + 0.5);
		texColor = texture(textures[tid], fs_in.uv);
	}

	color = texColor * intensity;
}