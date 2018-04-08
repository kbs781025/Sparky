#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices = 3) out;

in VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}gs_in[];

out VS_OUT
{
	vec3 Normal;
	vec3 FragPos;
	vec2 TexCoord;
}gs_out;

vec3 getNormal()
{
	vec3 dir1 = vec3(gl_in[1].gl_Position) - vec3(gl_in[0].gl_Position);
	vec3 dir2 = vec3(gl_in[2].gl_Position) - vec3(gl_in[0].gl_Position);
	return normalize(cross(dir2, dir1));
};

uniform float time;

vec4 explode(vec4 position, vec3 normal)
{
	float magnitude = 2.0;
	vec3 dir = normal * (sin(time) + 1.0) / 2.0 * magnitude;
	return position + vec4(dir, 0.0);
};

void main()
{
	vec3 normal = getNormal();
	for(int i = 0; i < 3; i ++)
	{
		gl_Position = explode(gl_in[i].gl_Position, normal);
		gs_out.Normal = gs_in[i].Normal;
		gs_out.FragPos = gs_in[i].FragPos;
		gs_out.TexCoord = gs_in[i].TexCoord;
		EmitVertex();
	}
}