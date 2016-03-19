#include "PlanetTest.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <string>

#include <glm/gtx/transform.hpp>
#include <iostream>

#include "Camera.h"
#include "Input.h"
#include "World.h"

#include "GUI.h"

PlanetTest::PlanetTest() {
	planet = Planet();
	planet.SetUp(4);

	Camera *cam = Camera::GetInstance();
	cam->SetPosition(0, 0, 10);
	
	camRotX = 0;
	camRotY = 0;
	camZoom = 10;

	selector = HexSelector();
}

PlanetTest::~PlanetTest() {
	
}


void PlanetTest::Update() {
	Camera *cam = Camera::GetInstance();

	// controls
	Input *input = Input::GetInstance();
	float dragSpeed = 5 * camZoom/8 * input->DeltaTime() / planet.GetRadius() * 4;

	if (input->MouseButton(2)) {
		if (input->MouseButtonDown(2)) {
			input->BindCursor();
		}

		vec2 drag = input->MouseDelta() * dragSpeed;

		camRotX -= drag.x * dragSpeed;
		if (camRotX > 2*M_PI) camRotX -= 2*M_PI;
		if (camRotX < 0) camRotX += 2*M_PI;

		camRotY -= drag.y * dragSpeed;
		if (camRotY > 2*M_PI/5) camRotY = 2*M_PI/5;
		if (camRotY < -2*M_PI/5) camRotY = -2*M_PI/5;
	} else if (input->MouseButtonUp(2)) {
		input->UnbindCursor();
	}

	camZoom -= 20 * input->MouseScroll() * input->DeltaTime();
	if (camZoom < planet.GetRadius() * 2) camZoom = planet.GetRadius() * 2;
	else if (camZoom > planet.GetRadius() * 4) camZoom = planet.GetRadius() * 4;

	// control results
	quat camRot = quat(vec3(camRotY, camRotX, 0));
	vec3 camPos = camRot * vec3(0, 0, camZoom);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);

	// set hex selector
	if (!Input::IsCursorBound()) {
		glm::vec3 hitPos;

		if (planet.GetRayHit(Cursor::GetRay(), hitPos)) {
			PlanetHex *hex = planet.GetClosestHexToPos(hitPos);
			selector.SetTarget(hex);
		} else {
			selector.ReleaseTarget();
		}
	}
}

void PlanetTest::Draw() {
	planet.Draw();

	// draw selector
	if (!Input::IsCursorBound()) {
		selector.Draw();

		FontManager *fontMgr = World::GetFontManager();
		if (selector.GetTarget() != nullptr) {
			fontMgr->WriteLine(
				fontMgr->GetAtlas("times.ttf"), 
				planet.GetType()->hexes[selector.GetTarget()->temp][selector.GetTarget()->height].c_str(), 
				5, 5
				);
		} else {
			fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), "No Hit", 5, 5);
		}
	}
}