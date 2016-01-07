#version 330

uniform bool textured;

uniform sampler2D image;

in vec2 vertUV;
in vec4 inColour;

out vec4 outputColour;

void main() 
{
	outputColour = inColour;
	
	if (textured) {
		vec4 imgColour = texture(image, vertUV);
		outputColour = imgColour * inColour;
	}
}