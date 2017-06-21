#version 330 core

in VS_OUT
{
	vec2 texcoord;
	vec4 particleColor;
}fs_in;

uniform sampler2D sprite;

out vec4 FragColor;

void main()
{
	FragColor = texture2D(sprite, fs_in.texcoord) * fs_in.particleColor;
}