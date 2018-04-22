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
uniform sampler2DShadow shadowMap;
uniform vec3 lightPosition;

float calculate_shadow_factor(vec4 light_space_pos, vec3 normal, vec3 lightDir);

void main()
{
	vec3 diffuseColor = vec3(texture(diffuseMap, fs_in.texCoord));
	vec3 ambientColor = diffuseColor * 0.2;
	vec3 normal = normalize(fs_in.normal);
	vec3 lightDir = normalize(lightPosition - fs_in.fragPosition);
	float diffuse = max(dot(normal, lightDir), 0.0);
	diffuseColor *= diffuse;

	float shadowFactor = calculate_shadow_factor(fs_in.posLight, normal, lightDir);

	fragColor = vec4(diffuseColor * shadowFactor + ambientColor, 1.0);
}

float calculate_shadow_factor(vec4 light_space_pos, vec3 normal, vec3 lightDir)
{
	vec3 projCoord = light_space_pos.xyz / light_space_pos.w;
	if(projCoord.z > 1.0)
	{
		return 1.0;
	}

	projCoord = projCoord * 0.5 + 0.5;
	float currentDepth = projCoord.z;
	float minOffset = 0.005;
	float maxSlopeOffset = 0.05;
	float offsetMod = 1.0 - max(dot(normal, lightDir), 0.0);
	float offset = max(minOffset, maxSlopeOffset * offsetMod);

	float shadow = 0.0;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int y = -1; y <=1; y++)
	{
		for(int x = -1; x <=1; x++)
		{
			vec3 UVC = vec3(projCoord.xy + vec2(x, y) * texelSize, projCoord.z - offset);
			shadow += texture(shadowMap, UVC);
		}
	}
	shadow /= 9.0;

	return shadow;
}