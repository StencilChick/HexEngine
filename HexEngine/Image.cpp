#include "Image.h"

#include <SOIL\SOIL.h>
#include <iostream>

Image::Image() {}
Image::Image(const char *file) {
	imageData = SOIL_load_image(file, &width, &height, 0, SOIL_LOAD_RGBA);
}
Image::Image(unsigned char *data, int width, int height) {
	this->imageData = data;
	this->width = width;
	this->height = height;
}
void Image::Delete() {
	SOIL_free_image_data(imageData);
}


// getting stuff
int Image::GetWidth() {
	return width;
}
int Image::GetHeight() {
	return height;
}

unsigned char* Image::GetImageData() {
	return imageData;
}