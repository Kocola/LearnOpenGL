#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 1) in vec3 vNormal;
layout (location = 2) in vec2 vTexcoord;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out vec3 fPosition;
out vec2 fTexcoord;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	
	fPosition = vPosition;
	
	fTexcoord = vTexcoord;
}
