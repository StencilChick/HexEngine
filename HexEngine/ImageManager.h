#pragma once

#include <map>
#include <string>

#include <GL/glew.h>

class ImageManager {
public:
	ImageManager();

	void Load();
	void Unload();

	GLuint GetImage(std::string);
	GLuint GetImage(const char*);

private:
	std::map<std::string, GLuint> images;
};