#version 330 core
out vec4 FragColor;

struct Light
{
	vec4 lightColor;
	vec4 position;
	vec4 direction;
	vec4 attenuation;
} pointLight;

layout (binding = GBUFFER_POS) uniform sampler2D texture_pos;
layout (binding = GBUFFER_NORMAL) uniform sampler2D texture_normal;
layout (binding = GBUFFER_ALBEDO_SPECULAR) uniform sampler2D texture_color;
layout (location = ) uniform vec3 view_pos;
layout (location = WINDOW_SIZE_LOCATION) uniform vec2 window_size;

float shininess = 20.0;

//vec3 CalcTangentPointLight(vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcTangentPointLight(vec2 TexCoord);
vec2 CalcTexCoord();

void main()
{
	vec2 texCoord = CalcTexCoord();
	vec3 result = CalcTangentPointLight(texCoord);

	FragColor = vec4(result, 1.0);
}

vec3  CalcTangentPointLight(vec2 TexCoord)
{
	vec3 normal = vec3(texture(texture_normal, TexCoord));
	normal = normal * 2.0 - 1.0;
	normal = normalize(normal);

	vec3 fragPos = texture(texture_pos, TexCoord).rgb;

	vec3 lightDir = normalize(pointLight.position.rgb - fragPos);
	float diffuse = max(dot(lightDir, normal), 0.0);
	vec4 color = texture(texture_color, TexCoord);
	vec3 albedo = pow(color.rgb, vec3(2.2));
	
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 viewDir = view_pos - fragPos;
	float specularIntensity = color.a;
	float specular = pow(max(dot(reflectDir, viewDir), 0.0), shininess);

	float dist = length(pointLight.position.rgb - fragPos);
	float attenuation = 1.0 / (pointLight.attenuation.r + pointLight.attenuation.g * dist + pointLight.attenuation.b * dist * dist);

	vec3 lightColor = pointLight.lightColor.rgb;
	vec3 ambient = lightColor * 0.1 * albedo;
	diffuse *= lightColor * 0.7 * albedo;
	specular *= lightColor * 1.0 * specularIntensity;

	vec3 result = (ambient + diffuse + specular) * attenuation;

	return pow(result, vec3(1.0/2.2));
}

vec2 CalcTexCoord()
{
	return gl_FragCoord.xy / window_size;
}