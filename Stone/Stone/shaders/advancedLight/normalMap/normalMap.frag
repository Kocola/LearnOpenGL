#version 330 core

in VS_OUT
{
	mat3 TBN;
	vec3 position;
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
	vec3 normal = texture2D(normalMap, fs_in.texcoord).rgb;
	normal = normalize(normal * 2.0f - 1.0f);
	normal = normalize(fs_in.TBN * normal);
	
	vec3 texColor = texture2D(diffuseTexture, fs_in.texcoord).rgb;
	
	vec3 ambient = texColor * 0.1f;
	
	vec3 lightDir = normalize(lightPos - fs_in.position);
	float diff = max(dot(lightDir, normal), 0.0f);
	vec3 diffuse = diff * texColor;
	
	vec3 viewDir = normalize(viewPos - fs_in.position);
	vec3 halfwayDir = normalize(lightDir + viewDir);
	float spec = pow(max(dot(halfwayDir, normal), 0.0f), 32.0f);
	vec3 specular = spec * vec3(1.0f);
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(result, 1.0f);
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0 - texcoord.y);
}