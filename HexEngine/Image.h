#pragma once

#include <GL/glew.h>

class Image {
public:
	Image();
	Image(const char*);
	Image(unsigned char*, int, int);
	void Delete();

	void BindGL();

	int GetWidth();
	int GetHeight();

	unsigned char* GetImageData();

private:
	unsigned char *imageData;
	int width, height;
};