#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 modelMatrix[128];
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 colour[128];

out vec2 vertUV;
out vec4 inColour;

void main()
{
	vertUV = uv;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix[gl_InstanceID] * vec4(position, 1);
	
	inColour = colour[gl_InstanceID];
}