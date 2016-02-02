#include "PlanetTest.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

#include "Camera.h"
#include "Input.h"
#include "World.h"

#include "GUI.h"

PlanetTest::PlanetTest() {
	planet = Planet();
	planet.SetUp(2);

	Camera *cam = Camera::GetInstance();
	cam->SetPosition(0, 0, 10);
	
	camRotX = 0;
	camRotY = 0;
	camZoom = 10;
}

PlanetTest::~PlanetTest() {
	
}


void PlanetTest::Update() {
	Camera *cam = Camera::GetInstance();

	// controls
	Input *input = Input::GetInstance();
	float dragSpeed = 3 * camZoom/8 * input->DeltaTime();

	if (input->MouseButton(2)) {
		if (input->MouseButtonDown(2)) {
			input->BindCursor();
		}

		vec2 drag = input->MouseDelta();

		camRotX -= drag.x * dragSpeed;
		if (camRotX > 2*M_PI) camRotX -= 2*M_PI;
		if (camRotX < 0) camRotX += 2*M_PI;

		camRotY -= drag.y * dragSpeed;
		if (camRotY > 2*M_PI/5) camRotY = 2*M_PI/5;
		if (camRotY < -2*M_PI/5) camRotY = -2*M_PI/5;
	} else if (input->MouseButtonUp(2)) {
		input->UnbindCursor();
	}

	camZoom -= input->MouseScroll();
	if (camZoom < 8) camZoom = 8;
	else if (camZoom > 20) camZoom = 20;

	// control results
	quat camRot = quat(vec3(camRotY, camRotX, 0));
	vec3 camPos = camRot * vec3(0, 0, camZoom);
	cam->SetPosition(camPos);
	cam->SetRotation(camRot);
}

void PlanetTest::Draw() {
	planet.Draw();

	/*GUI::DrawImage(
		World::GetImageManager()->GetImage("testGradient.png"),
		0, 0,
		128, 128
		);*/
}