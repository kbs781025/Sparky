#version 330 core
out vec4 FragColor;

in DATA
{
	vec2 TexCoord;
} fs_in;

uniform sampler2D u_Texture;

void main()
{
    FragColor = texture(u_Texture, fs_in.TexCoord);
}  