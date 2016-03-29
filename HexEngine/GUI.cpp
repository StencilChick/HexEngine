#include "GUI.h"

#include <glm/gtx/transform.hpp>
using namespace glm;

#include "World.h"

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
		1 - (y * 1.0f / World::SCREEN_HEIGHT * 2)
		);
}
vec2 GUI::ScreenPosToGL(vec2 pos) { return ScreenPosToGL((int)pos.x, (int)pos.y); }

vec2 GUI::ScreenSizeToGL(int w, int h) {
	return vec2(
		w * 1.0f / World::SCREEN_WIDTH, 
		h * 1.0f / World::SCREEN_HEIGHT
		) * 2.0f;
}
vec2 GUI::ScreenSizeToGL(vec2 pos) { return ScreenSizeToGL((int)pos.x, (int)pos.y); }

vec2 GUI::GLToScreenPos(float x, float y) {
	return vec2(
		(x + 1) * World::SCREEN_WIDTH / 2,
		(y - 1) * World::SCREEN_HEIGHT / 2
		);
}
vec2 GUI::GLToScreenPos(glm::vec2 pos) { return GLToScreenPos(pos.x, pos.y); }


// images
void GUI::DrawImage(Image *image, int x, int y, int width, int height, bool usingDepth) {
	float depth;
	usingDepth ? depth = GetDepth() : 0.1f;

	Mesh *m = World::GetMeshManager()->GetMesh("z_plane.obj");
	m->Draw(
		World::GetShaderManager()->GetShader("hud"),
		translate(vec3(ScreenPosToGL(x+width/2, y+height/2), depth)) * scale(vec3(ScreenSizeToGL(width, height)/2.0f, 1)),
		vec4(1, 1, 1, 1),
		image
		);
}
void GUI::DrawImage(Image *image, int x, int y, int width, int height) { DrawImage(image, x, y, width, height, true); }