#version 330 core

in VS_OUT
{
	vec2 texcoord;
}fs_in;

uniform sampler2D gPosition;
uniform sampler2D gNormal;
uniform sampler2D gAlbedoSpec;

struct Light
{
	vec3 position;
	vec3 color;
	float radius;
	float linear;
	float quadratic;
};

const int NR_LIGHTS = 32;
uniform Light lights[NR_LIGHTS];
uniform vec3 viewPos;

out vec4 color;

void main()
{
	//从G缓冲中获取数据
	vec3 position = texture2D(gPosition, fs_in.texcoord).rgb;
	vec3 normal = texture2D(gNormal, fs_in.texcoord).rgb;
	vec3 albedo = texture2D(gAlbedoSpec, fs_in.texcoord).rgb;
	
	//和往常一样进行光照计算
	vec3 lighting = vec3(0.0f);
	vec3 viewDir = normalize(viewPos - position);
	for(int i = 0; i < NR_LIGHTS; ++i)
	{
		float distance = length(lights[i].position - position);
		float attenuation = 1.0f / (1.0f + lights[i].linear * distance + 
			lights[i].quadratic * distance * distance);
			
		lighting += attenuation * albedo * vec3(0.1) * lights[i].color;	//环境光
	
		//漫反射
		vec3 lightDir = normalize(lights[i].position - position);
		float diff = max(dot(lightDir, normal), 0.0f);
		vec3 diffuse = attenuation * diff * albedo * lights[i].color;
		lighting += diffuse;
		
		//镜面高光
		vec3 halfwayDir = normalize(viewDir + lightDir);
		float spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
		vec3 specular = attenuation * spec * lights[i].color;
		lighting += specular;
	}
	
	color = vec4(vec3(lighting), 1.0f);
}