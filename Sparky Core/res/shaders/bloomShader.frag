out vec4 FragColor;

in vec2 TexCoords;

layout(binding = 0) uniform sampler2D hdrImage;
uniform bool horizontal;
uniform float weight[5] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main()
{
	vec3 hdrTexel = texture(hdrImage, TexCoords).rgb;
	vec2 texOffset = 1.0 / textureSize(hdrImage, 0);
	hdrTexel *= hdrTexel * weight[0];
	for(int i = 1; i < 5; ++i)
	{
		if(horizontal)
		{
			hdrTexel += texture(hdrImage, TexCoords + vec2(texOffset.x * i, 0)).rgb * weight[i];
			hdrTexel += texture(hdrImage, TexCoords - vec2(texOffset.x * i ,0)).rgb * weight[i];
		}
		else
		{
			hdrTexel += texture(hdrImage, TexCoords + vec2(0, texOffset.y * i)).rgb * weight[i];
			hdrTexel += texture(hdrImage, TexCoords - vec2(0, texOffset.y * i)).rgb * weight[i];
		}
	}

	FragColor = vec4(hdrTexel, 1.0);
}