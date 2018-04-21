#version 330 core
out vec4 FragColor;

in VS_OUT
{
	vec2 TexCoord;
} fs_in;

uniform sampler2D depthMap;
float far_plane = 100.0;
float near_plane = 0.1;

float LinearizeDepth(float depth)
{
    float z = depth * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}

void main()
{
	float depthValue = texture(depthMap, fs_in.TexCoord).r;
	depthValue = LinearizeDepth(depthValue)/far_plane;
    FragColor = vec4(vec3(depthValue), 1.0);
}  