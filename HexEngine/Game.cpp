#include "Game.h"

#include <GL/glew.h>

#include "Camera.h"
#include "Input.h"

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
	Input *input = Input::GetInstance();

	float speed = 1.0f * input->DeltaTime();
	if (input->GetKey(GLFW_KEY_RIGHT)) {
		g.Rotate(-speed, vec3(0, 1, 0));
	} if (input->GetKey(GLFW_KEY_LEFT)) {
		g.Rotate(speed, vec3(0, 1, 0));
	} if (input->GetKey(GLFW_KEY_UP)) {
		g.Rotate(speed, vec3(1, 0, 0));
	} if (input->GetKey(GLFW_KEY_DOWN)) {
		g.Rotate(-speed, vec3(1, 0, 0));
	}
}

void Game::Draw() {
	g.Draw();
}