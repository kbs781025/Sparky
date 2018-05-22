out vec4 FragColor;

in VS_DATA
{
	vec2 TexCoord;
	vec3 TangentFragPos;
	vec3 TangentViewPos;
	vec3 TangentLightPos;
} fs_in;

struct Light
{
	vec4 lightColor;
	vec4 position;
	vec4 direction;
	vec4 attenuation;
};

layout (binding = DIFFUSE_TEXTURE_BINDING) uniform sampler2D texture_diffuse;
layout (binding = SPEUCLAR_TEXTURE_BINDING) uniform sampler2D texture_specular;
layout (binding = NORMAL_TEXTURE_BINDING) uniform sampler2D texture_normal;
layout (binding = NORMAL_TEXTURE_BINDING) uniform sampler2D texture_depth;

#define NR_POINT_LIGHTS 1
layout(std140, binding = LIGHT_UNIFORM_BLOCK_BINDING) uniform Lights
{
	Light lights[NR_POINT_LIGHTS];
};

layout (location = SPECULAR_SHININESS_LOCATION) uniform float shininess;
layout (location = PARALLAX_SCALE_LOCATION) uniform float parallax_scale;

// Function Prototypes
vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir);
vec2 SteepParallaxMapping(vec2 texCoord, vec3 viewDir);
vec2 ReliefParallaxMapping(vec2 currentTex, vec2 currentDeltaTex, float currentDepth, float layerHeight);
vec2 ParallaxOcclusionMapping(vec2 currentTex, vec2 currentDeltaTex, float currentDepth, float layerHeight);

vec2 ParallaxMapping(vec2 texCoord, vec3 viewDir)
{
	float depth = texture(texture_depth, texCoord).r;
	vec2 texOffset = parallax_scale * viewDir.xy / viewDir.z * depth;
	// vec2 texOffset = parallax_scale * viewDir.xy * depth;

	return texCoord - texOffset;
}

vec2 SteepParallaxMapping(vec2 texCoord, vec3 viewDir)
{
	const float minLayers = 5.0;
	const float maxLayers = 15.0;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));

	float layerHeight = 1.0 / numLayers;
	vec2 dTex = parallax_scale * (viewDir.xy / viewDir.z) / numLayers;

	float currentDepth = 0.0;
	float sampleDepth = texture(texture_depth, texCoord).r;
	while(currentDepth < sampleDepth)
	{
		currentDepth += layerHeight;
		texCoord -= dTex;
		sampleDepth = texture(texture_depth, texCoord).r;
	}

	// Relief Parallax mapping
	ReliefParallaxMapping(texCoord, dTex, currentDepth, layerHeight);

	return texCoord;
}

vec2 ReliefParallaxMapping(vec2 currentTex, vec2 currentDeltaTex, float currentDepth, float layerHeight)
{
	currentDeltaTex /= 2.0;
	layerHeight /= 2.0;
	currentTex += currentDeltaTex;
	currentDepth -= layerHeight;

	const int numSearches = 5;
	for(int i = 0; i < numSearches; i++)
	{
		currentDeltaTex /= 2.0;
		layerHeight /= 2.0;
		float sampleDepth = texture(texture_depth, currentTex).r;
		if(currentDepth < sampleDepth) // above the surface
		{
			currentTex -= currentDeltaTex;
			currentDepth += layerHeight;
		}
		else // below the surface
		{
			currentTex += currentDeltaTex;
			currentDepth -= layerHeight;
		}
	}

	return currentTex;
}

vec2 ParallaxOcclusionMapping(vec2 currentTex, vec2 currentDeltaTex, float currentDepth, float layerHeight)
{
	return vec2(1.0);
}

float ParallaxSoftShadowMultiplier(Light light, vec2 texCoord, float initialHeight)
{
	return 0.0;
}

vec4 NormalMappingLighting(Light light, vec3 viewDir, vec2 texCoord, float shadowMultiplier)
{
	vec3 normal = normalize(texture(texture_normal, texCoord).xyz * 2.0 - 1.0);
	vec3 lightDir = normalize(fs_in.TangentLightPos - fs_in.TangentFragPos);
	vec3 diffuse = texture(texture_diffuse, texCoord).rgb;
	diffuse = pow(diffuse, vec3(2.2));

	float iamb = 0.2;
	float idiff = max(dot(normal, lightDir), 0.0);
	float ispec = 0.0;
	if(dot(normal, lightDir) > 0.2)
	{
		vec3 reflectDir = reflect(-lightDir, normal);
		ispec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);
	}

	float dist = distance(fs_in.TangentLightPos, fs_in.TangentFragPos);
	float attenuation = light.attenuation.r + light.attenuation.g * dist + light.attenuation.b * dist * dist;

	vec3 ambientColor = light.lightColor.rgb * diffuse * iamb;
	vec3 diffuseColor = light.lightColor.rgb * diffuse * idiff;
	vec3 specularColor = light.lightColor.rgb * ispec;

	vec4 resColor;
	resColor.rgb = (ambientColor + (diffuseColor /*+ specularColor*/) * pow(shadowMultiplier, 32.0)) / attenuation;
	resColor.rgb = pow(resColor.rgb, vec3(1.0/2.2));
	resColor.a = 1.0;

	return resColor;
}

void main()
{
	vec3 viewDir = normalize(fs_in.TangentViewPos - fs_in.TangentFragPos);
	vec2 texCoord = ParallaxMapping(fs_in.TexCoord, viewDir);

	float shadowFactor = 1.0;

	FragColor = NormalMappingLighting(lights[0], viewDir, fs_in.TexCoord, shadowFactor);
}

