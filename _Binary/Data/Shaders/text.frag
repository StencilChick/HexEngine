#version 330

uniform vec4 colour = vec4(1, 1, 1, 1);
uniform sampler2D image;

in vec2 vertUV;

out vec4 outputColour;

void main() 
{
	outputColour = vec4(1, 1, 1, texture(image, vertUV).w) * colour;
}