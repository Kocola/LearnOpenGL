#version 330 core

in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

out vec4 color;

void main()
{
	float lightDistance = length(FragPos.xyz - lightPos);
	lightDistance = lightDistance / far_plane;
	gl_FragDepth = lightDistance;
	color = vec4(vec3(lightDistance), 1.0f);
}