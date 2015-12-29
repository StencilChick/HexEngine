#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

out vec2 vertUV;

void main()
{
	vertUV = uv;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(position, 1);
}