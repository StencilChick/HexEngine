#include "FontManager.h"

#include "World.h"
#include "GUI.h"

#include <algorithm>
#include <iostream>
using namespace std;

#include <glm/glm.hpp>
using namespace glm;

#include "FileLoaders.h"
#include "ShaderHelpers.h"

FontManager::FontManager() {
	if (FT_Init_FreeType(&ft)) {
		cout << "Cannot load freetype library" << endl;
	}
}

// load/unload
void FontManager::Load() {
	// set up some things
	shader = World::GetShaderManager()->GetShader("text");
	SetShaderV4(shader, "colour", vec4(1, 1, 1, 1));

	// load the files
	vector<string> files = ListFilesAtDirectory(".\\Data\\Fonts\\*.ttf");

	int subPos = files[0].find("Fonts\\") + 6;

	for (int i = 0; i < files.size(); i++) {
		string name = files[i].substr(subPos);
		name = name.substr(0, name.size()-1);

		atlases[name] = CharAtlas();
		if (FT_New_Face(ft, ("./Data/Fonts/"+name).c_str(), 0, &atlases[name].face)) {
			cout << "Cannot load font '" << name << "'\n";
		}
		SetAtlasSize(&atlases[name], 18);
	}
}

void FontManager::Unload() {
	for (map<string, CharAtlas>::iterator it = atlases.begin(); it != atlases.end(); it++) {
		glDeleteTextures(1, &it->second.image);
	}
}


// get a character atlas
CharAtlas* FontManager::GetAtlas(string name) {
	return &atlases[name];
}
CharAtlas* FontManager::GetAtlas(const char* name) { return GetAtlas(string(name)); }


// write stuff
void FontManager::WriteLine(CharAtlas *atlas, const char *text, int x, int y) {
	GLuint vao;
	GLuint buffer;

	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	vector<float> verts = TextToMeshPoints(atlas, text, x, y);

	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * verts.size(), &verts.front(), GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);

	glUseProgram(shader);
	glBindTexture(GL_TEXTURE_2D, atlas->image);
	glDrawArrays(GL_TRIANGLES, 0, verts.size()/5);

	glDeleteBuffers(1, &buffer);
	glDeleteVertexArrays(1, &vao);
}


// things
vector<float> FontManager::TextToMeshPoints(CharAtlas *a, const char *input, int x, int y) {
	vector<float> verts;

	float depth = GUI::GetDepth();

	vec2 pos = GUI::ScreenPosToGL(x, y+a->size);

	for (int i = 0; i < strlen(input); i++) {
		CharInfo c = a->characters[input[i]];

		vec2 newPos = pos + GUI::ScreenSizeToGL(c.bx, c.by);
		vec2 size = GUI::ScreenSizeToGL(c.bw, c.bh);

		float x2 = newPos.x;
		float y2 = newPos.y;
		float w = size.x;
		float h = size.y;

		pos += GUI::ScreenSizeToGL(c.ax, c.ay);

		if (!w || !h) continue;

		verts.push_back(x2); verts.push_back(y2); verts.push_back(depth); verts.push_back(c.tx); verts.push_back(0);
		verts.push_back(x2); verts.push_back(y2-h); verts.push_back(depth); verts.push_back(c.tx); verts.push_back(-c.bh/a->height);
		verts.push_back(x2+w); verts.push_back(y2-h); verts.push_back(depth); verts.push_back((c.tx+c.bw/a->width)); verts.push_back(-c.bh/a->height);
		
		verts.push_back(x2+w); verts.push_back(y2-h); verts.push_back(depth); verts.push_back((c.tx+c.bw/a->width)); verts.push_back(-c.bh/a->height);
		verts.push_back(x2+w); verts.push_back(y2); verts.push_back(depth); verts.push_back((c.tx+c.bw/a->width)); verts.push_back(0);
		verts.push_back(x2); verts.push_back(y2); verts.push_back(depth); verts.push_back(c.tx); verts.push_back(0);
	}
	
	return verts;
}

void FontManager::SetAtlasSize(CharAtlas *a, int size) {
	FT_Set_Pixel_Sizes(a->face, 0, size);
	a->size = size;

	MakeAtlasImage(a);
}

void FontManager::MakeAtlasImage(CharAtlas *a) {
	// atlas stuff
	FT_GlyphSlot g = a->face->glyph;

	// get width and height
	int width = 0, height = 0;
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(a->face, i, FT_LOAD_RENDER)) {
			cout << "There was an error loading character '" << i << "'\n";
			continue;
		}

		width += g->bitmap.width;
		height = std::max(height, (int)g->bitmap.rows);
	}
	a->width = width;
	a->height = height;

	// make a texture
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &a->image);
	glBindTexture(GL_TEXTURE_2D, a->image);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA, width, height, 0, GL_ALPHA, GL_UNSIGNED_BYTE, 0);

	// fill it
	int x = 0;
	for (int i = 32; i < 128; i++) {
		if (FT_Load_Char(a->face, i, FT_LOAD_RENDER)) {
			continue;
		}

		glTexSubImage2D(GL_TEXTURE_2D, 0, x, 0, g->bitmap.width, g->bitmap.rows, GL_ALPHA, GL_UNSIGNED_BYTE, g->bitmap.buffer);

		// charinfo stuff
		a->characters[i].ax = g->advance.x >> 6;
		a->characters[i].ay = g->advance.y >> 6;

		a->characters[i].bx = g->bitmap_left;
		a->characters[i].by = g->bitmap_top;
		a->characters[i].bw = g->bitmap.width;
		a->characters[i].bh = g->bitmap.rows;

		a->characters[i].tx = x * 1.0f / a->width;

		x += g->bitmap.width;
	}
}