#include "GridTest.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <time.h>
#include <string>

#include "World.h"
#include "Input.h"
#include "Camera.h"

#include <iostream>

GridTest::GridTest() {
	srand(time(NULL));
	seed = rand() % 100000;

	grid = Grid();
	grid.SetUp(seed, 256, 160);

	Camera *cam = Camera::GetInstance();
	cam->SetPosition(0, 2, 0);
	cam->SetRotation(M_PI/2, vec3(1, 0, 0));
}


void GridTest::Update() {
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

	/*if (input->GetKeyDown(GLFW_KEY_RIGHT) || input->GetKeyDown(GLFW_KEY_D)) {
		pos.x++;
		if (pos.x >= grid.GetWidth()) pos.x = 0;
	}
	if (input->GetKeyDown(GLFW_KEY_LEFT) || input->GetKeyDown(GLFW_KEY_A)) {
		pos.x--;
		if (pos.x < 0) pos.x = grid.GetWidth()-1;
	}
	if (input->GetKeyDown(GLFW_KEY_UP) || input->GetKeyDown(GLFW_KEY_W)) {
		pos.y--;
		if (pos.y < 0) pos.y = grid.GetHeight()-1;
	}
	if (input->GetKeyDown(GLFW_KEY_DOWN) || input->GetKeyDown(GLFW_KEY_S)) {
		pos.y++;
		if (pos.y >= grid.GetHeight()) pos.y = 0;
	}
	grid.SetBlockPos(pos);*/
}

void GridTest::Draw() {
	grid.Draw();

	FontManager *fontMgr = World::GetFontManager();
	fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), ("Seed: " + std::to_string(seed)).c_str(), 5, 5);
	//fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), ("Block: <" + std::to_string((int)pos.x) + ", " + std::to_string((int)pos.y) + ">").c_str(), 5, 25);
}