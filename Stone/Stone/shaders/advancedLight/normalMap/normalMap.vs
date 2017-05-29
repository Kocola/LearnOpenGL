#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords;
layout (location = 3) in vec3 tangent;
layout (location = 4) in vec3 bitangent;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out VS_OUT
{
	mat3 TBN;
	vec3 position;
	vec2 texcoord;
}vs_out;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	vs_out.position = vec3(model * vec4(position, 1.0f));
	vs_out.texcoord = texcoords;
	
	mat3 normalMatrix = transpose(inverse(mat3(model)));
	
	vec3 T = normalize(normalMatrix * tangent);
	vec3 B = normalize(normalMatrix * bitangent);
	vec3 N = normalize(normalMatrix * normal);
	
	vs_out.TBN = mat3(T, B, N);
}