#version 330 core
out vec4 fragColor;

in VS_OUT
{
	vec4 posLight;
	vec3 fragPosition;
	vec3 normal;
	vec2 texCoord;
}fs_in;

uniform sampler2D diffuseMap;
uniform sampler2D shadowMap;
uniform vec3 lightPosition;

void main()
{
	vec3 diffuseColor = vec3(texture(diffuseMap, fs_in.texCoord));
	vec3 ambientColor = diffuseColor * 0.4;
	vec3 normal = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPosition - fs_in.fragPosition);
	float diffuse = max(dot(normal, lightDir), 0.0);
	diffuseColor *= diffuse;

	vec3 projCoord = fs_in.posLight.xyz / fs_in.posLight.w;
	projCoord = projCoord * 0.5 + 0.5;
	float closeDepth = texture(shadowMap, projCoord.xy).r;
	float currentDepth = projCoord.z;
	float bias = 0.005;
	if(currentDepth > closeDepth + bias)
	{
		diffuseColor *= 0.0;
	}

	fragColor = vec4(diffuseColor + ambientColor, 1.0);
}
