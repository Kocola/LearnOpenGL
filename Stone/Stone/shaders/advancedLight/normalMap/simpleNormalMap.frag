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
	//从法线贴图范围[0，1]获取法线
	vec3 normal = texture(normalMap, fs_in.texcoord).rgb;
	//将法线向量转换为范围[-1, 1]
	normal = 2 * normal - 1.0;
	
	vec3 texColor = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	
	//环境光
	vec3 ambient = texColor * 0.1;
	
	//漫反射光
	vec3 viewDir = normalize(viewPos - fs_in.worldPosition);
	float diff = max(dot(viewDir, normal), 0.0);
	vec3 diffuse = diff * texColor;
	
	//镜面高光
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