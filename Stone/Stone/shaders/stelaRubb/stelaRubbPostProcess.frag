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
	
	vec2 offsets[25] = vec2[](
		vec2(2 * -offsetX, 2 * offsetY),		vec2(-offsetX, 2 * offsetY),		vec2(0, 2 * offsetY),		vec2(offsetX, 2 * offsetY),		vec2(2 * offsetX, 2 * offsetY),
		vec2(2 * -offsetX, 	  offsetY),		vec2(-offsetX, 	     offsetY),		vec2(0, 	  offsetY),		vec2(offsetX, 	   offsetY),		vec2(2 * offsetX, 	     offsetY),
		vec2(2 * -offsetX, 			   0),		vec2(-offsetX,                0),		vec2(0, 			   0),		vec2(offsetX,               0),		    vec2(2 * offsetX,                0),
		vec2(2 * -offsetX, 	 -offsetY),		vec2(-offsetX, 	    -offsetY),		vec2(0, 	 -offsetY),		vec2(offsetX, 	  -offsetY),		vec2(2 * offsetX, 	    -offsetY),
		vec2(2 * -offsetX,-2 * offsetY),		vec2(-offsetX,-2 * offsetY),		vec2(0,-2 * offsetY),		vec2(offsetX,-2 * offsetY),		vec2(2 * offsetX,-2 * offsetY)
	);
	
//	float kernel[9] = float[](
//		-1, -1, -1,
//		-1,  9, -1,
//		-1, -1, -1
//	);

	float kernel[25] = float[](
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, -24, 1, 1,
		1, 1, 1, 1, 1,
		1, 1, 1, 1, 1
	);
	
//	vec3 sampleTex[25];
//	for(int i = 0; i < 25; ++i)
//	{
//		sampleTex[i] = vec3(texture2D(colorTexture, fs_in.texcoord.st + offsets[i]));
//	}
	
//	vec3 col = vec3(0.0f);
//	for(int i = 0; i < 25; ++i)
//	{
//		col += sampleTex[i] * kernel[i];
//	}

	const int sizeLen = 7;
	const int total = sizeLen * sizeLen;

	vec3 sampleTex[total];
	for(int i = 0; i < total; ++i)
	{
		sampleTex[i] = vec3(texture2D(colorTexture, fs_in.texcoord.st + vec2((i / sizeLen - sizeLen / 2) * offsetX, (i % sizeLen - sizeLen / 2) * offsetY)));
	}
	
	vec3 col = vec3(0.0f);
	for(int i = 0; i < total; ++i)
	{
		if(i == total / 2)
		{
			col += sampleTex[i] * -(total - 1);
		}
		else
		{
			col += sampleTex[i] * 1;
		}
	}
	
	color = vec4(col, 1.0f);
	
}