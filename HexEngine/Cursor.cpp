#include "Cursor.h"

#include "World.h"
#include "GUI.h"
#include "Camera.h"

#include <iostream>

// singleton stuff
Cursor* Cursor::instance = nullptr;
Image* Cursor::image = nullptr;
glm::vec2 Cursor::pos = glm::vec2(0, 0);

Cursor* Cursor::GetInstance() {
	if (instance == nullptr) {
		instance = new Cursor();
	}

	return instance;
}

void Cursor::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

// constructor
Cursor::Cursor() {
	image = World::GetImageManager()->GetImage("HUD\\cursor.png");
}

Cursor::~Cursor() {

}


void Cursor::Draw() {
	Input *input = Input::GetInstance();

	if (!input->IsCursorBound()) {
		pos = input->MousePos();
	}

	GUI::DrawImage(
		image,
		pos.x, pos.y,
		image->GetWidth(), image->GetHeight()
		);
}


Ray Cursor::GetRay() {
	glm::vec4 near = Camera::GetInverseViewMatrix() * Camera::GetInverseProjectionMatrix() * glm::vec4(Input::MouseGLPos(), 0, 1);
	near = glm::vec4(near.x/near.w, near.y/near.w, near.z/near.w, 1);

	return Ray((glm::vec3)near, glm::normalize((glm::vec3)near - Camera::GetInstance()->GetPosition()));
}