#version 330 core

layout (location = 0) in vec3 v_position;
layout (location = 1) in vec3 v_color;

uniform mat4 model;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

out vec3 f_color;

void main()
{
	vec4 position = projection * view * model * vec4(v_position, 1.0f);
	gl_Position = position;//vec4(position.x, position.y, 0, 1.0f);
	f_color = v_color;
}