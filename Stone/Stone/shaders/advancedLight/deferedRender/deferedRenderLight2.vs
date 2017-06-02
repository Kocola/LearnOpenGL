#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texcoord;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT
{
	vec2 texcoord;
}vs_out;

void main()
{
	vec4 position = projection * view * model * vec4(position, 1.0f);
	gl_Position = position;
	vs_out.texcoord = vec2(position.x / position.w, position.y / position.w) * 0.5 + 0.5;
}