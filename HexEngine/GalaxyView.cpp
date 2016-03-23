#include "GalaxyView.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Game.h"
#include "Camera.h"
#include "Input.h"

#include "Generic.h"

GalaxyView::GalaxyView() {
	galaxy = Game::GetGalaxy();

	camDist = 10;
	camRotX = 0;
	camRotY = -M_PI/4;
}


void GalaxyView::Update() {
	Camera *cam = Camera::GetInstance();

	// camera controls
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

	camDist -= 20 * input->MouseScroll() * input->DeltaTime();
	if (camDist < 2) camDist = 2;
	else if (camDist > 20) camDist = 20;

	quat camRot = quat(vec3(camRotY, camRotX, 0));
	vec3 camPos = camRot * vec3(0, 0, camDist);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);
}

void GalaxyView::Draw() {
	galaxy->Draw();
}