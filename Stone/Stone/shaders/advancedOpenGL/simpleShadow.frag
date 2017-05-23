#version 330 core

in VS_OUT
{
	vec3 fPosition;
	vec3 fNormal;
	vec2 fTexcoord;
	vec4 posLightSpace;
} fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D shadowMap;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 FragColor;

vec2 reverseY(vec2 texcoord);

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir);

void main()
{
	vec3 color = texture2D(diffuseTexture, reverseY(fs_in.fTexcoord)).rgb;
	vec3 normal = normalize(fs_in.fNormal);
	vec3 lightColor = vec3(1.0f);
	//环境光
	vec3 ambient = 0.15 * color;
	//漫反射光
	vec3 lightDir = normalize(lightPos - fs_in.fPosition);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * color;
	//镜面高光
	vec3 viewDir = normalize(viewPos - fs_in.fPosition);
	vec3 reflectDir = reflect(-lightDir, normal);
	float spec = 0.0f;
	vec3 halfwayDir = normalize(lightDir + viewDir);
	spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
	vec3 specular = spec * lightColor;
	//计算阴影
	float shadow = shadowCalculation(fs_in.posLightSpace, fs_in.fNormal, lightDir);
	vec3 lighting = (ambient + (1.0 - shadow) * (diffuse + specular)) * color;
	
	FragColor = vec4(lighting, 1.0f);
	//FragColor = vec4( (ambient + (diffuse + specular)) * color, 1.0f);
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0f - texcoord.y);
}

float shadowCalculation(vec4 fragPosLightSpace, vec3 normal, vec3 lightDir)
{
	//执行透视除法
	vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
	projCoords = projCoords * 0.5 + 0.5;
	float closeDepth = texture2D(shadowMap, projCoords.xy).r;
	float currentDepth = projCoords.z;
	float bias = max(0.05, 0.05 * (1 - dot(normal, lightDir)));
	//float shadow = currentDepth - bias> closeDepth ? 1.0 : 0.0;
	
	float shadow = 0.0f;
	vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
	for(int x = -1; x <= 1; ++x)
	{
		for(int y = -1; y <= 1; ++y)
		{
			float pcfDepth = texture2D(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
			shadow += currentDepth - bias > pcfDepth ? 1.0 : 0.0;
		}
	}
	shadow /= 9.0f;
	
	//if(currentDepth > 1.0)
	//	shadow = 0.0;
	return shadow;
}