out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D hdrImage;
layout(binding = 1) uniform sampler2D bloomImage;
uniform float exposure;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture(hdrImage, TexCoords).rgb;
	vec3 bloomColor = texture(bloomImage, TexCoords).rgb;
	hdrColor += bloomColor;
	// reinhard
	//vec3 result = hdrColor / (hdrColor + vec3(1.0));

	// exposure
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0 / gamma));
	FragColor = vec4(result, 1.0);
}