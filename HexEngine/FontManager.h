#pragma once

#include <map>
#include <string>
#include <vector>

#include "FontHelpers.h"

#include <vector>

class FontManager {
public:
	FontManager();

	void Load();
	void Unload();

	CharAtlas* GetAtlas(std::string);
	CharAtlas* GetAtlas(const char*);

	void WriteLine(CharAtlas*, const char*, int, int);

private:
	FT_Library ft;
	GLuint shader;

	std::map<std::string, CharAtlas> atlases;

	std::vector<float> TextToMeshPoints(CharAtlas*, const char*, int, int);
	void MakeAtlasImage(CharAtlas*);
	void SetAtlasSize(CharAtlas*, int);
};