#version 330 core
out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoord;
} fs_in;

struct Material
{
	sampler2D texture_diffuse1;
};

uniform Material material;

void main()
{
    FragColor = texture(material.texture_diffuse1, fs_in.TexCoord);
}  