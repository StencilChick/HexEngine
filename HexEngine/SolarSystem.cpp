#include "SolarSystem.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include <iostream>

#include <glm/gtx/transform.hpp>

#include "World.h"
#include "Game.h"
#include "Camera.h"
#include "GUI.h"
#include "Cursor.h"

SolarSystem::SolarSystem(Star *star) : Scene() {
	this->star = star;
	for (int i = 0; i < star->GetPlanetCount(); i++) {
		star->GetPlanet(i)->SetUpMesh();
	}

	mode = Mode::solar;

	// planet stuff
	hexSelector = HexSelector();

	// camera values
	maxDistPlanet = 28;
	maxDistSolar = star->GetPlanet(star->GetPlanetCount()-1)->GetDistanceToOrbitCentre() * 3; //2048;
	minDistSolar = 512;

	camDist = maxDistSolar - 100;
	camRotX = 0;
	camRotY = -M_PI / 4;
	camFocus = glm::vec3(0, 0, 0);
}

SolarSystem::~SolarSystem() {
	for (int i = 0; i < star->GetPlanetCount(); i++) {
		star->GetPlanet(i)->DeleteMesh();
	}
}


void SolarSystem::Update() {
	Game::GetGalaxy()->Update();

	// modes
	if (mode == Mode::solar) {
		UpdateCamSolar();
	} else {
		UpdateCamPlanet();
	}
	Camera::GetInstance()->Update();

	if (mode == Mode::solar) {
		UpdateControlsSolar();
	} else {
		UpdateControlsPlanet();
	}

	// hud
	Game::GetGameHUD()->Update();
}

void SolarSystem::Draw() {
	World::GetMeshManager()->GetMesh("sphere.obj")->Draw(
		World::GetShaderManager()->GetShader("default"),
		glm::scale(glm::vec3(64, 64, 64)),
		star->GetColour(),
		World::GetImageManager()->GetImage("white.png")//"Planets\\gasTest.png")
		);

	for (int i = 0; i < star->GetPlanetCount(); i++) {
		Planet *p = star->GetPlanet(i);

		p->Draw();
		p->DrawOrbit((450+camDist-minDistSolar)/(maxDistSolar-minDistSolar));

		for (int i = 0; i < p->GetMoonCount(); i++) {
			Planet *m = p->GetMoon(i);

			m->Draw();
			m->DrawOrbit((450+camDist-minDistSolar)/(maxDistSolar-minDistSolar));
		}
	}

	if (mode == Mode::planet && !Input::IsCursorBound()) {
		hexSelector.Draw();
	}

	Game::GetGameHUD()->Draw();
}


void SolarSystem::SwitchToPlanetMode() {
	if (targetPlanet != nullptr) {
		if (!targetPlanet->IsGasPlanet()) {
			mode = Mode::planet;

			camRotX -= targetPlanet->GetRotation();
			camDist = maxDistPlanet-3;
		}
	}
}

void SolarSystem::SwitchToSolarMode() {
	mode = Mode::solar;

	camRotX += targetPlanet->GetRotation();
	camDist = (minDistSolar-maxDistPlanet)/2;

	Game::GetGameHUD()->ReleaseTarget();
}


void SolarSystem::UpdateControlsSolar() {
	// select a planet
	if (Input::MouseButtonDown(0)) {
		if (targetPlanet == nullptr) {
			// target a planet
			TargetLoop();
		} else {
			// zoom in on target planet
			if (targetPlanet->GetRayHit(Cursor::GetRay())) {
				SwitchToPlanetMode();
			} else {
				// target a different planet perhaps
				TargetLoop();
			}
		}
	}
}

void SolarSystem::TargetLoop() {
	Planet *newTarget = nullptr;

	Camera *cam = Camera::GetInstance();

	for (int i = 0; i < star->GetPlanetCount(); i++) {
		Planet *p = star->GetPlanet(i);

		if (p->GetRayHit(Cursor::GetRay())) {
			if (newTarget != nullptr) {
				if (glm::length(p->GetPosition() - cam->GetPosition()) < glm::length(newTarget->GetPosition() - cam->GetPosition())) {
					newTarget = p;
				}
			} else {
				newTarget = p;
			}
		}

		// moons
		for (int i = 0; i < p->GetMoonCount(); i++) {
			Planet *m = p->GetMoon(i);

			if (m->GetRayHit(Cursor::GetRay())) {
				if (newTarget != nullptr) {
					if (glm::length(m->GetPosition() - cam->GetPosition()) < glm::length(newTarget->GetPosition() - cam->GetPosition())) {
						newTarget = m;
					}
				} else {
					newTarget = m;
				}
			}
		}
	}

	if (newTarget != nullptr) {
		targetPlanet = newTarget;
	}
}

void SolarSystem::UpdateControlsPlanet() {
	if (!Input::IsCursorBound()) {
		glm::vec3 hitPos;

		/*if (targetPlanet->GetRayHit(Cursor::GetRay(), hitPos)) {
			// transform hit position to planet's local space
			hitPos -= targetPlanet->GetPosition();

			float rot = targetPlanet->GetRotation();
			hitPos = glm::inverse(glm::quat(cos(rot/2), 0, sin(rot/2), 0)) * hitPos;

			hitPos = glm::vec3(round(hitPos.x*100), round(hitPos.y*100), round(hitPos.z*100)) / 100.0f;


			PlanetHex *hex = targetPlanet->GetClosestHexToPos(hitPos);
			hexSelector.SetTarget(hex);
			Game::GetGameHUD()->SetTargetHex(hex);
		} else {
			hexSelector.ReleaseTarget();
			Game::GetGameHUD()->ReleaseTarget();
		}*/
	}
}


void SolarSystem::UpdateCamSolar() {
	Camera *cam = Camera::GetInstance();

	Input *input = Input::GetInstance();
	float dragSpeed = 1 * Input::DeltaTime();

	// rotation
	camRotY = -(M_PI/2 - M_PI*3/8 + M_PI*3/8 * (camDist - minDistSolar)/(maxDistSolar-minDistSolar));
	if (input->MouseButton(2)) {
		if (input->MouseButtonDown(2)) {
			input->BindCursor();
		}

		float drag = input->MouseDelta().x * dragSpeed;

		camRotX -= drag;
		if (camRotX > 2*M_PI) camRotX -= 2*M_PI;
		if (camRotX < 0) camRotX += 2*M_PI;
	} else if (input->MouseButtonUp(2)) {
		input->UnbindCursor();
	}

	// position
	if (targetPlanet != nullptr) {
		camFocus = targetPlanet->GetPosition();
	}

	float moveSpeed = (200 + 300 * (camDist - minDistSolar) / (maxDistSolar - minDistSolar)) * Input::DeltaTime();
	if (Input::GetKey(GLFW_KEY_W) || Input::GetKey(GLFW_KEY_UP)) {
		vec3 v = cam->GetForward();
		v.y = 0;
		v = normalize(v);

		camFocus -= v * moveSpeed;

		targetPlanet = nullptr;
	}
	if (Input::GetKey(GLFW_KEY_S) || Input::GetKey(GLFW_KEY_DOWN)) {
		vec3 v = cam->GetForward();
		v.y = 0;
		v = normalize(v);

		camFocus += v * moveSpeed;

		targetPlanet = nullptr;
	}
	if (Input::GetKey(GLFW_KEY_A) || Input::GetKey(GLFW_KEY_LEFT)) {
		vec3 v = cam->GetRight();
		v.y = 0;
		v = normalize(v);

		camFocus -= v * moveSpeed;

		targetPlanet = nullptr;
	}
	if (Input::GetKey(GLFW_KEY_D) || Input::GetKey(GLFW_KEY_RIGHT)) {
		vec3 v = cam->GetRight();
		v.y = 0;
		v = normalize(v);

		camFocus += v * moveSpeed;

		targetPlanet = nullptr;
	}
	if (glm::length(camFocus) > star->GetPlanet(star->GetPlanetCount()-1)->GetDistanceToOrbitCentre()) {
		camFocus = normalize(camFocus) * star->GetPlanet(star->GetPlanetCount()-1)->GetDistanceToOrbitCentre();
	}

	// distance
	camDist -= (500 + std::max(camDist-minDistSolar, 0.0f)) * input->MouseScroll() * input->DeltaTime();
	if (camDist < minDistSolar) {
		if (targetPlanet != nullptr) {
			if (camDist < (minDistSolar-maxDistPlanet)/2) {
				SwitchToPlanetMode();
				return;
			}
		} else {
			camDist = minDistSolar;
		}
	}
	else if (camDist > maxDistSolar) {
		Game::PopScene();
	}

	// do the thing
	quat camRot = quat(vec3(camRotY, camRotX, 0));
	vec3 camPos = camFocus + camRot * vec3(0, 0, camDist);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);
}

void SolarSystem::UpdateCamPlanet() {
	Camera *cam = Camera::GetInstance();

	camFocus = targetPlanet->GetPosition();

	// rotation
	float dragSpeed = 5 * Input::DeltaTime();
	if (Input::MouseButton(2)) {
		if (Input::MouseButtonDown(2)) {
			Input::BindCursor();
		}

		vec2 drag = Input::MouseDelta() * dragSpeed;

		camRotX -= drag.x * dragSpeed;
		if (camRotX > 2*M_PI) camRotX -= 2*M_PI;
		if (camRotX < 0) camRotX += 2*M_PI;

		camRotY -= drag.y * dragSpeed;
		if (camRotY > M_PI * 2/5) camRotY = M_PI * 2/5;
		if (camRotY < -M_PI * 2/5) camRotY = -M_PI * 2/5;
	} else if (Input::MouseButtonUp(2)) {
		Input::UnbindCursor();
	}

	// distance
	const int minDist = targetPlanet->GetRadius()*2.5f;
	camDist -= 30 * Input::MouseScroll() * Input::DeltaTime();
	if (camDist < minDist) camDist = minDist;
	else if (camDist > maxDistPlanet) {
		SwitchToSolarMode();
		return;
	}

	// do the thing
	quat camRot = quat(vec3(camRotY, camRotX + targetPlanet->GetRotation(), 0));
	vec3 camPos = camFocus + camRot * vec3(0, 0, camDist);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);
}