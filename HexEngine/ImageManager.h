#pragma once

#include <map>
#include <string>

#include "Image.h"

class ImageManager {
public:
	ImageManager();

	void Load();
	void Unload();

	Image* GetImage(std::string);
	Image* GetImage(const char*);

private:
	std::map<std::string, Image> images;
};