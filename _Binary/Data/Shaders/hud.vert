#version 330

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 uv;

uniform mat4 modelMatrix[128];

uniform vec4 colour[128];

uniform int textureID[128];
uniform int w[128];
uniform int h[128];

out vec2 vertUV;
out vec4 inColour;
flat out int imgID;

void main() {
	vertUV = vec2(uv.x, 1 - uv.y);
	gl_Position = modelMatrix[gl_InstanceID] * vec4(position, 1);
	
	inColour = colour[gl_InstanceID];
	
	imgID = textureID[gl_InstanceID];
}