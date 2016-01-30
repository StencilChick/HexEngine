#pragma once

#include <GL/glew.h>

class Image {
public:
	Image();
	Image(const char*);
	void Delete();

	void BindGL();

	GLuint GetIndex();
	int GetWidth();
	int GetHeight();

	unsigned char* GetImageData();

private:
	GLuint tex;
	int width, height;

	unsigned char *imageData;
};