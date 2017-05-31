#version 330 core

in vec3 worldPosition;
in vec3 worldNormal;
in vec2 texcoord;

struct LightColor
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

struct LightAttenuation
{
	float constant;
	float linear;
	float quadratic;
};

uniform sampler2D 				tex;
uniform LightColor		 		lightColor;
uniform LightAttenuation 	lightAttenuation;
uniform float							shininess;

uniform vec3 		lightPos;
uniform vec3 		viewPos;

out vec4 color;

vec2 reverseY(vec2 texcoord);

void main()
{
	//环境光
	vec3 ambient = lightColor.ambient;
	
	//漫反射光
	vec3 norm = normalize(worldNormal);
	vec3 lightDir = normalize(lightPos - worldPosition);
	float diff = max(dot(norm, lightDir), 0.0f);
	//vec3 diffuse = lightColor.diffuse * (diff * material.diffuse);
	vec3 diffuse = lightColor.diffuse * (diff * vec3(texture2D(tex, reverseY(texcoord))));
	
	//镜面高光
	vec3 viewDir = normalize(viewPos - worldPosition);
	vec3 reflectDir = reflect(-lightDir, norm);
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), shininess);
	vec3 specular = lightColor.specular * spec * vec3(texture2D(tex, reverseY(texcoord)));
	
	vec3 result = ambient + diffuse + specular;
	color = vec4(vec3(result), 1.0f);
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0f - texcoord.y);
}