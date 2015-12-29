#include "Game.h"

#include <GL/glew.h>

#include "Camera.h"

#include "Mesh.h"
#include "ShaderHelpers.h"
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

Game::Game() {
	g = Generic();
	g.Tranlsate(0, 0, -2);
	g.SetScale(vec3(0.25f, 0.25f, 0.25f));
}

Game::~Game() {

}

// singleton stuff
Game* Game::instance = nullptr;
Game* Game::GetInstance() {
	if (instance == nullptr) {
		instance = new Game();
	}

	return instance;
}
void Game::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}


// draw and update
void Game::Update() {
	//Camera::GetInstance()->Rotate(0.0005f, vec3(0, 1, 0));
	g.Rotate(0.0005f, vec3(1, 1, 1));
}

void Game::Draw() {
	g.Draw();
}