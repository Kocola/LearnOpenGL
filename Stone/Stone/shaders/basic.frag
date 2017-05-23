#version 330 core

uniform sampler2D diffuseTexture;

in vec3 fPosition;
in vec2 fTexcoord;

out vec4 color;

vec2 reverseY(vec2 texcoord);

void main()
{
	color = texture2D(diffuseTexture, reverseY(fTexcoord));
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0f - texcoord.y);
}