#version 330

uniform bool textured;

uniform sampler2DArray image;

in vec2 vertUV;
in vec4 inColour;
flat in int imgID;

out vec4 outputColour;

void main() 
{
	outputColour = texture(image, vec3(vertUV, imgID)) * inColour;
}