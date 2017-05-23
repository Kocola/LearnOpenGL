#version 330 core

in vec2 fTexcoord;

uniform sampler2D screenTexture;

out vec4 color;

void main()
{
	color = vec4(texture2D(screenTexture, fTexcoord).r);
}