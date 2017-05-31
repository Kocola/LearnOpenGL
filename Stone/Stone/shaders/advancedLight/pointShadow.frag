#version 330 core

in VS_OUT
{
	vec3 FragPos;
	vec3 Normal;
	vec2 Texcoord;
} fs_in;

uniform sampler2D diffuseTexture;
uniform samplerCube depthMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

uniform float far_plane;

uniform float reverseNormal; 

out vec4 FragColor;

vec2 reverseY(vec2 texcoord);
float shadowCalculation(vec3 fragPos);

void main()
{
	vec3 color = texture2D(diffuseTexture, reverseY(fs_in.Texcoord)).rgb;
	vec3 normal = normalize(fs_in.Normal) * reverseNormal;
	vec3 lightColor = vec3(1.0f);
	//������
	vec3 ambient = 0.05 * color;
	//�������
	vec3 lightDir = normalize(lightPos - fs_in.FragPos);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * lightColor;
	//����߹�
	vec3 viewDir = normalize(viewPos - fs_in.FragPos);
	vec3 reflectDir = reflect(-lightDir, normal);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
	vec3 specular = spec * lightColor;
	//������Ӱ
	float shadow = shadowCalculation(fs_in.FragPos);
	vec3 lightting = (ambient + (1.0f - shadow) * (diffuse + specular)) * color; 
	
	FragColor = vec4(vec3(lightting), 1.0f);
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0f - texcoord.y);
}

float shadowCalculation(vec3 fragPos)
{
	vec3 fragToLight = fragPos - lightPos;
	float closeDepth = texture(depthMap, fragToLight).r;
	closeDepth *= far_plane;
	float currentDepth = length(fragToLight);
	float bias = 0.00;
	
	float shadow = currentDepth - bias > closeDepth ? 1.0 : 0.0;

	return shadow;
}