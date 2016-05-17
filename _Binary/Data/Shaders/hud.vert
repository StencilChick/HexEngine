#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;
layout (location = 2) in vec3 normal;

uniform mat4 modelMatrix[64];

uniform vec4 colour[64];

uniform int textureID[64];
uniform int w[64];
uniform int h[64];

out vec2 vertUV;
out vec4 inColour;
flat out int imgID;

flat out int texWidth;
flat out int texHeight;

void main() {
	vertUV = vec2(uv.x, 1 - uv.y);
	gl_Position = modelMatrix[gl_InstanceID] * vec4(position, 1);
	
	inColour = colour[gl_InstanceID];
	
	imgID = textureID[gl_InstanceID];
    
    texWidth = w[textureID[gl_InstanceID]];
    texHeight = h[textureID[gl_InstanceID]];
}