#include "Image.h"

#include <SOIL\SOIL.h>
#include <iostream>

Image::Image() {}
Image::Image(const char *file) {
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);

	imageData = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
	
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
}
void Image::Delete() {
	SOIL_free_image_data(imageData);
	glDeleteTextures(1, &tex);
}


void Image::BindGL() {
	glBindTexture(GL_TEXTURE_2D, tex);
}


// getting stuff
GLuint Image::GetIndex() {
	return tex;
}

int Image::GetWidth() {
	return width;
}
int Image::GetHeight() {
	return height;
}

unsigned char* Image::GetImageData() {
	return imageData;
}