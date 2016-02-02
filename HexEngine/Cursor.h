#pragma once

#include "Image.h"

#include <glm/glm.hpp>

class Cursor {
public:
	// singleton stuff
	static Cursor* GetInstance();
	static void Destroy();

	// functions
	void Draw();

private:
	// singleton stuff
	static Cursor *instance;

	Cursor();
	~Cursor();
	Cursor(Cursor const&);
	void operator=(Cursor const&);

	// actual values
	static Image *image;
	static glm::vec2 pos;
};