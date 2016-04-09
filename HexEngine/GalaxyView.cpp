#include "GalaxyView.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

#include "Game.h"
#include "Camera.h"
#include "Input.h"
#include "Cursor.h"

#include "SolarSystem.h"

GalaxyView::GalaxyView() {
	galaxy = Game::GetGalaxy();

	targetStar = galaxy->GetStar(0);

	camDist = 2;
	camRotX = 0;
	camRotY = -M_PI/4;
	camFocus = targetStar->GetPosition();
	camFocus.y = 0;
}


void GalaxyView::Update() {
	// update galaxy
	galaxy->Update();

	// controls
	if (!Input::IsCursorBound()) {
		if (Input::MouseButtonDown(0)) {
			Star *s = GetRayhitStar(Cursor::GetRay());
			if (s != nullptr) {
				if (s == targetStar) {
					GoToSystemView();
					return;
				} else {
					targetStar = s;

					camFocus = s->GetPosition();
					camFocus.y = 0;
				}
			}
		}
	}

	// camera controls
	UpdateCamera();
}

void GalaxyView::Draw() {
	galaxy->Draw();
}


void GalaxyView::UpdateCamera() {
	Camera *cam = Camera::GetInstance();

	Input *input = Input::GetInstance();
	float dragSpeed = 5 * Input::DeltaTime();

	const int minDist = 1;
	const int maxDist = 20;

	// rotation
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

	// position
	float moveSpeed = (2 + 8 * (camDist-minDist)/(maxDist-minDist)) * Input::DeltaTime();
	if (Input::GetKey(GLFW_KEY_W) || Input::GetKey(GLFW_KEY_UP)) {
		vec3 v = cam->GetForward();
		v.y = 0;
		v = normalize(v);

		camFocus -= v * moveSpeed;

		targetStar = nullptr;
	}
	if (Input::GetKey(GLFW_KEY_S) || Input::GetKey(GLFW_KEY_DOWN)) {
		vec3 v = cam->GetForward();
		v.y = 0;
		v = normalize(v);

		camFocus += v * moveSpeed;

		targetStar = nullptr;
	}
	if (Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_RIGHT)) {
		vec3 v = cam->GetRight();
		v.y = 0;
		v = normalize(v);

		camFocus += v * moveSpeed;

		targetStar = nullptr;
	}
	if (Input::GetKey(GLFW_KEY_A) || Input::GetKey(GLFW_KEY_LEFT)) {
		vec3 v = cam->GetRight();
		v.y = 0;
		v = normalize(v);

		camFocus -= v * moveSpeed;

		targetStar = nullptr;
	}

	// distance
	camDist -= 20 * input->MouseScroll() * input->DeltaTime();
	if (camDist < minDist) {
		if (targetStar == nullptr) {
			camDist = minDist;
		} else {
			GoToSystemView();
			camDist += 0.5f;
			return;
		}
	}
	else if (camDist > maxDist) camDist = maxDist;

	// do the thing
	quat camRot = quat(vec3(camRotY, camRotX, 0));
	vec3 camPos = camFocus + camRot * vec3(0, 0, camDist);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);
}


Star* GalaxyView::GetRayhitStar(Ray ray) {
	Star *star = nullptr;

	for (int i = 0; i < galaxy->GetStarCount(); i++) {
		Star *s = galaxy->GetStar(i);
		
		float a = glm::dot(ray.direction, ray.direction);
		float b = glm::dot(ray.direction, 2.0f*(ray.point-s->GetPosition()));
		float c = glm::dot(s->GetPosition(), s->GetPosition()) + glm::dot(ray.point, ray.point) - 2*glm::dot(s->GetPosition(), ray.point) - galaxy->starSize * galaxy->starSize;

		float d = b*b - 4*a*c;
		if (d >= 0) {
			star = s;
			break;
		}
	}

	return star;
}


void GalaxyView::GoToSystemView() {
	if (targetStar != nullptr) {
		Game::PushScene(new SolarSystem(targetStar));
	}
}