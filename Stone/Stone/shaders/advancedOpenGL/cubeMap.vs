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
	mat4 viewVar = mat4(mat3(view));	//�Ƴ�ƽ�Ʋ����������ƶ��Ͳ���Ӱ�쵽��պе�λ��������
	vec4 pos = projection * viewVar * model * vec4(position, 1.0f);\
	gl_Position = pos;
	texcoord = position;
}