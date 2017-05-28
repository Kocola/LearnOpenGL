#version 330 core

in VS_OUT
{
	vec3 worldPosition;
	vec2 texcoord;
}fs_in;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;

out vec4 color;

vec2 reverseY(vec2 texcoord);

void main()
{
	//�ӷ�����ͼ��Χ[0��1]��ȡ����
	vec3 normal = texture(normalMap, fs_in.texcoord).rgb;
	//����������ת��Ϊ��Χ[-1, 1]
	normal = 2 * normal - 1.0;
	
	vec3 texColor = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	
	//������
	vec3 ambient = texColor * 0.1;
	
	//�������
	vec3 viewDir = normalize(viewPos - fs_in.worldPosition);
	float diff = max(dot(viewDir, normal), 0.0);
	vec3 diffuse = diff * texColor;
	
	//����߹�
	vec3 lightDir = normalize(lightPos - fs_in.worldPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0), 32.0);
	vec3 specular = spec * vec3(1.0);
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0 - texcoord.y);
}