#version 330 core

layout (location = 0) in vec3 position;

layout (std140) uniform Camera
{
	mat4 view;
	mat4 projection;
};

uniform mat4 model;

out vec3 texcoord;

void main()
{
	mat4 viewVar = mat4(mat3(view));	//移除平移操作，这样移动就不会影响到天空盒的位置向量了
	vec4 pos = projection * viewVar * model * vec4(position, 1.0f);\
	gl_Position = pos;
	texcoord = position;
}