#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D scene;
uniform sampler2D bloomBlur;
uniform float exposure;

out vec4 color;

void main()
{
	const float gamma = 2.2f;
	vec3 hdrColor = texture2D(scene, fs_in.texcoord).rgb;
	vec3 bloomColor = texture2D(bloomBlur, fs_in.texcoord).rgb;
	hdrColor += bloomColor;
	
	vec3 result = vec3(1.0) - exp(-hdrColor * exposure);
	result = pow(result, vec3(1.0 / gamma));
	color = vec4(result, 1.0f);
}