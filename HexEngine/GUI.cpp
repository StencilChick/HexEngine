#include "GUI.h"

#include "World.h"

using namespace glm;

// values and junk
GUI* GUI::instance = nullptr;

float GUI::depth = 0;

// constructors and junk
GUI::GUI() {
	ResetDepth();
}

GUI::~GUI() {

}

// singleton stuff
GUI* GUI::GetInstance() {
	if (instance == nullptr) {
		instance = new GUI();
	}

	return instance;
}

void GUI::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}


// depth
float GUI::GetDepth() {
	if (depth > 0) depth -= 0.0001f;
	return depth + 0.0001f;
}

void GUI::ResetDepth() {
	depth = 0.0099f;
}


// screens and GLs and stuffs
vec2 GUI::ScreenPosToGL(int x, int y) {
	return vec2(
		-1 + (x * 1.0f / World::SCREEN_WIDTH * 2),
		1 - (y * 1.0f / World::SCREEN_HEIGHT * 2));
}
vec2 GUI::ScreenPosToGL(vec2 pos) { return ScreenPosToGL((int)pos.x, (int)pos.y); }

vec2 GUI::ScreenSizeToGL(int w, int h) {
	return vec2(
		w * 1.0f / World::SCREEN_WIDTH, 
		h * 1.0f / World::SCREEN_HEIGHT ) * 2.0f;
}