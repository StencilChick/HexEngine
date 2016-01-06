#pragma once

#include <GL/glew.h>

#include <ft2build.h>
#include FT_FREETYPE_H

struct CharInfo {
	// advanced coordinates
	float ax;
	float ay;

	// bitmap coordinates
	float bx;
	float by;
	float bw;
	float bh;

	// texture coordinates
	float tx;
};

struct CharAtlas {
	FT_Face face;

	int size;
	int width;
	int height;

	GLuint image;

	CharInfo characters[128];

	CharAtlas() { }
};