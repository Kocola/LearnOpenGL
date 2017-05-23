#version 330 core

out vec3 worldPosition;
//out vec3 worldNormal;
out vec2 texcoord;

uniform sampler2D tex;
uniform vec3 lightColor;

out vec4 color;

vec2 reverseY(vec2 texcoord);

void main()
{
	float ambientStrength = 0.5f;
	vec3 ambient = ambientStrength * lightColor;
	vec3 objectColor = texture2D(tex, reverseY(texcoord)).xyz;
	vec3 result = ambient * objectColor;
	color = vec4(result, 1.0f);
}

vec2 reverseY(vec2 texcoord)
{
	return vec2(texcoord.x, 1.0f - texcoord.y);
}