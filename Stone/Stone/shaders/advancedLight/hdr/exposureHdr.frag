#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D hdrBuffer;
uniform float exposure;

out vec4 color;

void main()
{
	const float gamma = 2.2;
	vec3 hdrColor = texture2D(hdrBuffer, fs_in.texcoord).rgb;
	//exposure
	vec3 mapped = vec3(1.0f) - exp(-hdrColor * exposure);
	//gamma
	vec3 result = pow(mapped, vec3(1.0f / gamma));
	color = vec4(result, 1.0f);
}