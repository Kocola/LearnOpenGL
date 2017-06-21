#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D image;
uniform vec3 spriteColor;

out vec4 FragColor;

void main()
{
	FragColor = vec4(spriteColor, 1.0f) * texture2D(image, fs_in.texcoord);
}