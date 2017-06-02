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
	//存储第一个G缓冲纹理片段中的片段位置向量
	gPosition = fs_in.position;
	//同样存储对每个逐片段法线到G缓冲中
	gNormal = normalize(fs_in.normal);
	//和漫反射对每个片段颜色存到G缓冲中
	gAlbedoSpec.rgb = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	//存储镜面强度到gAlbedoSpec的alpha分量
	gAlbedoSpec.a = texture2D(specularTexture, fs_in.texcoord).r;
	
	color = vec4(vec3(fs_in.texcoord, 0.0), 1.0f);//texture2D(diffuseTexture, fs_in.texcoord);
}