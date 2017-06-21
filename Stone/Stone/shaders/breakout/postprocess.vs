#version 330 core

layout (location = 0) in vec4 vertex; 	//<vec2 position, vec2 texcoord>

uniform bool chaos;
uniform bool confuse;
uniform bool shake;
uniform float time;

out VS_OUT
{
	vec2 texcoord;
}vs_out;

void main()
{
	gl_Position = vec4(vertex.xy, 0.0f, 1.0f);
	vec2 texture = vertex.zw;
	if(chaos)
	{
		float strength = 0.3f;
		vec2 pos = vec2(texture.x + sin(time) * strength, texture.y + cos(time) * strength);
		vs_out.texcoord = pos;
	}
	else if(confuse)
	{
		vs_out.texcoord = vec2(1.0f - texture.x, 1.0f - texture.y);
	}
	else
	{
		vs_out.texcoord = texture;
	}
	
	if(shake)
	{
		float strength = 0.006f;
		gl_Position.x += cos(time * 10) * strength;
		gl_Position.y += cos(time * 15) * strength;
	}
}