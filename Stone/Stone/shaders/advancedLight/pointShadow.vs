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

out VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoord;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	vs_out.FragPos = vec3(model * vec4(vPosition, 1.0f));
	vs_out.Normal = transpose(inverse(mat3(model))) * vNormal;
	vs_out.Texcoord = vTexcoord;
}