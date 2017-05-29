#version 330 core

in VS_OUT
{
	vec3 position;
	vec2 texcoord;
	vec3 tangentLightPos;
	vec3 tangentViewPos;
	vec3 tangentPosition;
}fs_in;

uniform sampler2D diffuseTexture;
uniform sampler2D normalMap;

out vec4 color;

void main()
{
	vec3 normal = texture2D(normalMap, fs_in.texcoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	
	vec3 texColor = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	
	vec3 ambient = texColor * 0.1f;
	
	vec3 lightDir = normalize(fs_in.tangentLightPos - fs_in.tangentPosition);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * texColor;
	
	vec3 viewDir = normalize(fs_in.tangentViewPos - fs_in.tangentPosition);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
	vec3 specular = spec * vec3(1.0f);
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}