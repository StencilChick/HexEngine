#include "HexSelector.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

#include <iostream>

#include "Planet.h"
#include "World.h"

HexSelector::HexSelector() {
	target = nullptr;
	mesh = nullptr;
}

HexSelector::~HexSelector() {
	if (mesh != nullptr) {
		delete mesh;
	}
}


void HexSelector::Draw() {
	if (mesh != nullptr) {
		float size = target->planet->GetRadius() + 0.0025f;

		mesh->Draw(
			World::GetShaderManager()->GetShader("default"),
			glm::translate(target->planet->GetPosition()) * scale(vec3(size, size, size)) * glm::rotate(target->planet->GetRotation(), glm::vec3(0, 1, 0)),
			vec4(0, 1, 1, 1),
			World::GetImageManager()->GetImage("white.png")
			);
		mesh->BindBuffersAndDraw();
	}
}


void HexSelector::SetTarget(PlanetHex *hex) {
	if (hex == target) return;

	target = hex;

	if (mesh != nullptr) delete mesh;

	std::vector<GLfloat> vertices;
	std::vector<GLushort> elements;
	hex->AddHexSurfaceToMesh(vertices, elements);

	mesh = new Mesh(&vertices[0], vertices.size()/5, &elements[0], elements.size());
	mesh->AddShader(World::GetShaderManager()->GetShader("default"));
}

void HexSelector::ReleaseTarget() {
	target = nullptr;

	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}

PlanetHex* HexSelector::GetTarget() {
	return target;
}