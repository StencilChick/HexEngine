#include "PlanetTest.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Input.h"
#include "World.h"

PlanetTest::PlanetTest() {
	planet = Planet();
	planet.SetUp(2);

	Camera *cam = Camera::GetInstance();
	cam->SetPosition(0, 0, 10);
}

PlanetTest::~PlanetTest() {
	
}


void PlanetTest::Update() {
	Camera *cam = Camera::GetInstance();

	Input *input = Input::GetInstance();
	float speed = 5 * input->DeltaTime();
	if (input->GetKey(GLFW_KEY_W)) {
		cam->TranslateLocal(0, 0, -speed);
	}
	if (input->GetKey(GLFW_KEY_A)) {
		cam->TranslateLocal(-speed, 0, 0);
	}
	if (input->GetKey(GLFW_KEY_S)) {
		cam->TranslateLocal(0, 0, speed);
	}
	if (input->GetKey(GLFW_KEY_D)) {
		cam->TranslateLocal(speed, 0, 0);
	}

	if (input->GetKey(GLFW_KEY_UP)) {
		cam->RotateLocal(-M_PI/4 * input->DeltaTime(), vec3(1, 0, 0));
	}
	if (input->GetKey(GLFW_KEY_DOWN)) {
		cam->RotateLocal(M_PI/4 * input->DeltaTime(), vec3(1, 0, 0));
	}
	if (input->GetKey(GLFW_KEY_LEFT)) {
		cam->RotateLocal(-M_PI/4 * input->DeltaTime(), vec3(0, 1, 0));
	}
	if (input->GetKey(GLFW_KEY_RIGHT)) {
		cam->RotateLocal(M_PI/4 * input->DeltaTime(), vec3(0, 1, 0));
	}
	if (input->GetKey(GLFW_KEY_Q)) {
		cam->RotateLocal(M_PI/4 * input->DeltaTime(), vec3(0, 0, -1));
	}
	if (input->GetKey(GLFW_KEY_E)) {
		cam->RotateLocal(-M_PI/4 * input->DeltaTime(), vec3(0, 0, -1));
	}
}

void PlanetTest::Draw() {
	planet.Draw();
}