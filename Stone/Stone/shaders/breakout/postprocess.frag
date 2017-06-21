#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D scene;
uniform vec2 			offsets[9];
uniform int				edgeKernel[9];
uniform float			blurKernel[9];

uniform bool chaos;
uniform bool confuse;
uniform bool shake;

out vec4 FragColor;

void main()
{
	FragColor = vec4(0.0f);
	
	vec3 sample[9];
	if(chaos || shake)
	{
		for(int i = 0; i < 9; ++i)
		{
			sample[i] = vec3(texture2D(scene, fs_in.texcoord.st + offsets[i]));
		}
	}
	
	//处理效果
	if(chaos)
	{
		for(int i = 0; i < 9; ++i)
		{
			FragColor += vec4(sample[i] * edgeKernel[i], 0.0f);
		}
		FragColor.a = 1.0f;
	}
	else if(confuse)
	{
		FragColor = vec4(1.0f - texture2D(scene, fs_in.texcoord).rgb, 1.0f);
	}
	else if(shake)
	{
		for(int i = 0; i < 9; ++i)
		{
			FragColor += vec4(sample[i] * blurKernel[i], 0.0f);
		}
		FragColor.a = 1.0f;
	}
	else
	{
		FragColor = texture2D(scene, fs_in.texcoord);
	}
}