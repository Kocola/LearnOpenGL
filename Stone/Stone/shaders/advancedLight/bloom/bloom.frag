#version 330 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in VS_OUT
{
	vec3 position;
	vec3 normal;
	vec2 texcoord;
}fs_in;

uniform sampler2D diffuseTexture;

uniform vec3 viewPos;
uniform vec3 lightPos;

//out vec4 color;

void main()
{
	vec3 texColor = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	vec3 normal = normalize(fs_in.normal);
	//������
	vec3 ambient = 0.0 * texColor;
	//�������
	vec3 lightDir = normalize(lightPos - fs_in.position);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * texColor;
	//����߹�
	vec3 viewDir = normalize(viewPos - fs_in.position);
	vec3 halfwayDir = normalize(viewDir + lightDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
	vec3 specular = spec *  vec3(1.0f);// * vec3(10.0f);
	
	vec3 result = ambient + diffuse + specular;
	
	//���result�Ƿ񳬳���ֵ������ǣ���ô�����Ϊ���⣨bloom������ֵ��ɫ
	float brightness = dot(result, vec3(0.2126, 0.7152, 0.0722));
	if(brightness > 1.0f)
	{
		BrightColor = vec4(result, 1.0f);
	}
	FragColor = vec4(result, 1.0f);
}