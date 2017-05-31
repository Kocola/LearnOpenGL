#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D image;
uniform bool horizontal;

uniform float weight[5] = float[](0.227027, 0.1945946, 0.1216216, 0.054054,
	0.016216);

out vec4 color;

void main()
{
	vec2 texOffset = 1.0f / textureSize(image, 0);
	vec3 result = texture2D(image, fs_in.texcoord).rgb * weight[0];
	if(horizontal)
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture2D(image, fs_in.texcoord + vec2(texOffset.x * i, 0.0)).rgb * weight[i];
			result += texture2D(image, fs_in.texcoord - vec2(texOffset.x * i, 0.0)).rgb * weight[i];
		}
	}
	else
	{
		for(int i = 1; i < 5; ++i)
		{
			result += texture2D(image, fs_in.texcoord + vec2(0.0, texOffset.y * i)).rgb * weight[i];
			result += texture2D(image, fs_in.texcoord - vec2(0.0, texOffset.y * i)).rgb * weight[i];
		}
	}
	color = vec4(result, 1.0f);
}