#version 330 core

layout (location = 0) in vec3 vPosition;
layout (location = 2) in vec2 vTexcoord;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT
{
	vec3 worldPosition;
	vec2 texcoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	vs_out.texcoord = vTexcoord;
	vs_out.worldPosition = vec3(model * vec4(vPosition, 1.0f));
}