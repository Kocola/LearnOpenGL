#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D colorTexture;

out vec4 color;

void main()
{
	//vec4(fs_in.texcoord.x, fs_in.texcoord.y, 0.0, 1.0f);
	color = texture2D(colorTexture, fs_in.texcoord);
	
	vec2 texelSize = 1.0f / textureSize(colorTexture, 0);
	float offsetX = texelSize.x;
	float offsetY = texelSize.y;
	
	vec2 offsets[9] = vec2[](
		vec2(-offsetX, offsetY), vec2(0, offsetY), vec2(offsetX, offsetY),
		vec2(-offsetX, 0.0f),	vec2(0.0f, 0.0f), vec2(offsetX, 0.0f),
		vec2(-offsetX, -offsetY), vec2(0.0f, -offsetY), vec2(offsetX, -offsetY)
	);
	
//	float kernel[9] = float[](
//		-1, -1, -1,
//		-1,  9, -1,
//		-1, -1, -1
//	);

	float kernel[9] = float[](
		1, 1, 1,
		1,  -8, 1,
		1, 1, 1
	);
	
	vec3 sampleTex[9];
	for(int i = 0; i < 9; ++i)
	{
		sampleTex[i] = vec3(texture2D(colorTexture, fs_in.texcoord.st + offsets[i]));
	}
	
	vec3 col = vec3(0.0f);
	for(int i = 0; i < 9; ++i)
	{
		col += sampleTex[i] * kernel[i];
	}
	
	color = vec4(col, 1.0f);
	
}