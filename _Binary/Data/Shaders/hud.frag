#version 330

uniform sampler2DArray image;

uniform int maxW;
uniform int maxH;
uniform int w;
uniform int h;

in vec2 vertUV;
in vec4 inColour;
flat in int imgID;

flat in int imgWidth;
flat in int imgHeight;

out vec4 outputColour;

void main() 
{
	outputColour = texture(image, vec3(vertUV, imgID) * vec3(imgWidth / maxW, imgHeight / maxH, 1)) * inColour;
}