#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

out vec2 vertUV;

void main()
{
	vertUV = uv * vec2(1, -1);
	gl_Position =  vec4(position, 1);
}