#version 330 core

in vec3 texcoord;

uniform samplerCube cubemap;

out vec4 color;

void main()
{
//	color = vec4(texcoord, 1.0f);
	color = texture(cubemap, texcoord);
}