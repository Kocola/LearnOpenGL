#version	330 core

layout (location = 0) in vec4 vertex;	//<vec2 position, vec2 texcoord>

uniform mat4 model;
uniform mat4 projection;

out VS_OUT
{
	vec2 texcoord;
}vs_out;

void main()
{
	vs_out.texcoord = vertex.zw;
	gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}