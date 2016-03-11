#version 330

uniform sampler2DArray image;

uniform int maxW;
uniform int maxH;

in vec2 vertUV;
in vec4 inColour;
flat in int imgID;

out vec4 outputColour;

void main() 
{
	vec3 size = textureSize(image, imgID);
	
	outputColour = texture(image, vec3(vertUV, imgID) * vec3(size.x/maxW, size.y/maxH, 1)) * inColour;
}