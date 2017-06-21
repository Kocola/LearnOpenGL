#version 330 core

layout (location = 0) in vec4 vertex;	//<vec2 position, vec2 texcoord>

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;

out VS_OUT
{
	vec2 texcoord;
	vec4 particleColor;
}vs_out;

void main()
{
	float scale = 2.8f;
	vs_out.texcoord = vertex.zw;
	vs_out.particleColor = color;
	gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0f);
}