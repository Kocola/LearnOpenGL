#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location =2 ) in vec2 texcoord;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.position = vec3(model * vec4(position, 1.0f));
	vs_out.texcoord = texcoord;
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	vs_out.normal = normalize(normalMatrix * normal);
}