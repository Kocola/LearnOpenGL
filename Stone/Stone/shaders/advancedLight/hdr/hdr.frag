#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D hdrBuffer;

out vec4 color;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture2D(hdrBuffer, fs_in.texcoord).rgb;
	//reinhard
	vec3 result = hdrColor / (hdrColor + vec3(1.0f));
	result = pow(result, vec3(1.0f / gamma));
	color = vec4(result, 1.0f);
}