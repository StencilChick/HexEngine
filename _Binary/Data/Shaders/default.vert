#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 modelMatrix[64];
uniform mat4 viewMatrix;
uniform mat4 projectionMatrix;

uniform vec4 colour[64];

uniform int textureID[64];
uniform int w[64];
uniform int h[64];

out vec2 vertUV;
out vec4 inColour;
flat out int imgID;

flat out int texWidth;
flat out int texHeight;

void main()
{
	vertUV = uv;
	gl_Position = projectionMatrix * viewMatrix * modelMatrix[gl_InstanceID] * vec4(position, 1);
	
	inColour = colour[gl_InstanceID];
	
	imgID = textureID[gl_InstanceID];
    
    texWidth = w[imgID];
    texHeight = h[imgID];
}