#version 330 core

layout (location = 0) out vec3 gPosition;
layout (location = 1) out vec3 gNormal;
layout (location = 2) out vec4 gAlbedoSpec;

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;

out vec4 color;

void main()
{
	//�洢��һ��G��������Ƭ���е�Ƭ��λ������
	gPosition = fs_in.position;
	//ͬ���洢��ÿ����Ƭ�η��ߵ�G������
	gNormal = normalize(fs_in.normal);
	//���������ÿ��Ƭ����ɫ�浽G������
	gAlbedoSpec.rgb = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	//�洢����ǿ�ȵ�gAlbedoSpec��alpha����
	gAlbedoSpec.a = texture2D(specularTexture, fs_in.texcoord).r;
	
	color = vec4(vec3(fs_in.texcoord, 0.0), 1.0f);//texture2D(diffuseTexture, fs_in.texcoord);
}