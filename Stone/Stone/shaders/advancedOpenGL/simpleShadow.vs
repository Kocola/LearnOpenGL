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
uniform mat4 lightSpaceMatrix;

out VS_OUT
{
	vec3 fPosition;
	vec3 fNormal;
	vec2 fTexcoord;
	vec4 posLightSpace;
} vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	vs_out.fPosition = vec3(model * vec4(vPosition, 1.0f));
	vs_out.fNormal = transpose(inverse(mat3(model))) * vNormal;
	vs_out.fTexcoord = vTexcoord;
	vs_out.posLightSpace = lightSpaceMatrix * vec4(vs_out.fPosition, 1.0f);
}