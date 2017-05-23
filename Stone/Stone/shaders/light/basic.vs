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

out vec3 worldPosition;
out vec3 worldNormal;
out vec2 texcoord;

void main()
{
	gl_Position = projection * view * model * vec4(vPosition, 1.0f);
	
	worldPosition 	= 	vec3(model * vec4(vPosition, 1.0f));
	worldNormal 		= 	transpose(inverse(mat3(model))) * vNormal;	//法向量从局部坐标系-->世界坐标系
	texcoord 			= 	vTexcoord;
}