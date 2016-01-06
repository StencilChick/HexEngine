#include "Test.h"

#include "World.h"
#include "Input.h"
#include "Camera.h"

#include <iostream>

Test::Test() {
	g = Generic();
	g.Tranlsate(0, 0, -2);
	g.SetScale(vec3(0.25f, 0.25f, 0.25f));
}


void Test::Update() {
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

void Test::Draw() {
	g.Draw();

	FontManager *fontMgr = World::GetFontManager();
	fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), "Arma virumque cano, Troiae qui primus ab oris Italiam fato profugus Laviniaque venit litora.", 5, 5);
}