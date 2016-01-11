#include "Test.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <time.h>
#include <string>

#include "World.h"
#include "Input.h"
#include "Camera.h"

#include <iostream>

Test::Test() {
	srand(time(NULL));
	seed = rand() % 100000;

	grid = Grid();
	grid.SetUp(seed, 256, 160);

	Camera *cam = Camera::GetInstance();
	cam->SetPosition(0, 5, 0);
	cam->SetRotation(M_PI/2, vec3(1, 0, 0));
}


void Test::Update() {
	grid.Update();

	Input *input = Input::GetInstance();

	Camera *cam = Camera::GetInstance();

	float speed = 3.0f * input->DeltaTime();
	if (input->GetKey(GLFW_KEY_RIGHT) || input->GetKey(GLFW_KEY_D)) {
		cam->Translate(speed, 0, 0);
	}
	if (input->GetKey(GLFW_KEY_LEFT) || input->GetKey(GLFW_KEY_A)) {
		cam->Translate(-speed, 0, 0);
	}
	if (input->GetKey(GLFW_KEY_UP) || input->GetKey(GLFW_KEY_W)) {
		cam->Translate(0, 0, -speed);
	}
	if (input->GetKey(GLFW_KEY_DOWN) || input->GetKey(GLFW_KEY_S)) {
		cam->Translate(0, 0, speed);
	}
	if (input->GetKey(GLFW_KEY_Q)) {
		cam->Translate(0, speed, 0);
	}
	if (input->GetKey(GLFW_KEY_E)) {
		cam->Translate(0, -speed, 0);
	}
}

void Test::Draw() {
	grid.Draw();

	FontManager *fontMgr = World::GetFontManager();
	fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), ("Seed: " + std::to_string(seed)).c_str(), 5, 5);
}