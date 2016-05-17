#version 330

uniform sampler2DArray image;

in vec2 vertUV;
in vec4 inColour;
flat in int imgID;

flat in int texWidth;
flat in int texHeight;

out vec4 outputColour;

void main() 
{
	vec3 maxSize = textureSize(image, 0);
    vec2 size = vec2(texWidth, texHeight);
	
	outputColour = texture(image, vec3(vertUV, imgID) * vec3(size.x/maxSize.x, size.y/maxSize.y, 1)) * inColour;
}