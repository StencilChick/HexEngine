#include "Test.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "World.h"
#include "Input.h"
#include "Camera.h"

#include <iostream>

Test::Test() {
	grid = Grid();
	grid.SetUp(10, 10);

	Camera *cam = Camera::GetInstance();
	cam->SetPosition(0, 5, 0);
	cam->SetRotation(M_PI/2, vec3(1, 0, 0));
}


void Test::Update() {
	grid.Update();

	Input *input = Input::GetInstance();

	Camera *cam = Camera::GetInstance();

	float speed = 3.0f * input->DeltaTime();
	if (input->GetKey(GLFW_KEY_RIGHT)) {
		cam->Translate(speed, 0, 0);
	}
	if (input->GetKey(GLFW_KEY_LEFT)) {
		cam->Translate(-speed, 0, 0);
	}
	if (input->GetKey(GLFW_KEY_UP)) {
		cam->Translate(0, 0, -speed);
	}
	if (input->GetKey(GLFW_KEY_DOWN)) {
		cam->Translate(0, 0, speed);
	}
}

void Test::Draw() {
	grid.Draw();

	//FontManager *fontMgr = World::GetFontManager();
	//fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), "Arma virumque cano, Troiae qui primus ab oris Italiam fato profugus Laviniaque venit litora.", 5, 5);
}