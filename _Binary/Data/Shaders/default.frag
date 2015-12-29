#version 330

uniform bool textured;
uniform vec4 colour;
uniform sampler2D image;

in vec2 vertUV;

out vec4 outputColour;

void main() 
{
	outputColour = colour;
	
	if (textured) {
		vec4 imgColour = texture(image, vertUV);
		outputColour = imgColour * colour;
	}
}