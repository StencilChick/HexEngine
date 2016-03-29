#include "SolarSystem.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

#include "World.h"
#include "Camera.h"

SolarSystem::SolarSystem(Star *star) : Scene() {
	this->star = star;
	for (int i = 0; i < star->GetPlanetCount(); i++) {
		star->GetPlanet(i)->SetUpMesh();
	}

	camDist = 50;
	camRotX = 0;
	camRotY = -M_PI / 4;
	camFocus = glm::vec3(0, 0, 0);
}


void SolarSystem::Update() {
	// camera controls
	Camera *cam = Camera::GetInstance();

	Input *input = Input::GetInstance();
	float dragSpeed = 5 * Input::DeltaTime();

	if (input->MouseButton(2)) {
		if (input->MouseButtonDown(2)) {
			input->BindCursor();
		}

		vec2 drag = input->MouseDelta() * dragSpeed;

		camRotX -= drag.x * dragSpeed;
		if (camRotX > 2*M_PI) camRotX -= 2*M_PI;
		if (camRotX < 0) camRotX += 2*M_PI;

		camRotY -= drag.y * dragSpeed;
		if (camRotY > -M_PI/12) camRotY = -M_PI/12;
		if (camRotY < -M_PI/2 * 0.85f) camRotY = -M_PI/2 * 0.85f;
	} else if (input->MouseButtonUp(2)) {
		input->UnbindCursor();
	}

	float moveSpeed = 500 * Input::DeltaTime();
	if (Input::GetKey(GLFW_KEY_W)) {
		vec3 v = cam->GetForward();
		v.y = 0;
		v = normalize(v);

		camFocus -= v * moveSpeed;
	}
	if (Input::GetKey(GLFW_KEY_S)) {
		vec3 v = cam->GetForward();
		v.y = 0;
		v = normalize(v);

		camFocus += v * moveSpeed;
	}
	if (Input::GetKey(GLFW_KEY_A)) {
		vec3 v = cam->GetRight();
		v.y = 0;
		v = normalize(v);

		camFocus -= v * moveSpeed;
	}
	if (Input::GetKey(GLFW_KEY_D)) {
		vec3 v = cam->GetRight();
		v.y = 0;
		v = normalize(v);

		camFocus += v * moveSpeed;
	}

	const int maxDist = 750;
	const int minDist = 250;
	camDist -= 500 * input->MouseScroll() * input->DeltaTime();
	if (camDist < minDist) camDist = minDist;
	else if (camDist > maxDist) camDist = maxDist;

	quat camRot = quat(vec3(camRotY, camRotX, 0));
	vec3 camPos = camFocus + camRot * vec3(0, 0, camDist);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);
}

void SolarSystem::Draw() {
	World::GetMeshManager()->GetMesh("sphere.obj")->Draw(
		World::GetShaderManager()->GetShader("default"),
		glm::scale(glm::vec3(32, 32, 32)),
		star->GetColour(),
		World::GetImageManager()->GetImage("white.png")
		);

	for (int i = 0; i < star->GetPlanetCount(); i++) {
		star->GetPlanet(i)->Draw();
	}
}