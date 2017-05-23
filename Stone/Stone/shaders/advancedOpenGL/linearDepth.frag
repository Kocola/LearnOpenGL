#version 330 core

out vec4 color;

float linearDepth(float depth);

void main()
{
	float depth = linearDepth(gl_FragCoord.z);
	color = vec4(vec3(depth), 1.0f);
}

float linearDepth(float depth)
{
	float near = 1.0f;
	float far = 100.0f;
	float z = depth * 2.0f - 1.0f;
	return (2.0f * near) / (far + near - z * (far - near));
}