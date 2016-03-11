#include "HexSelector.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

#include <iostream>

#include "Planet.h"
#include "World.h"

HexSelector::HexSelector() /*: GameObject("hexSelector.obj", "white.png")*/ {
	target = nullptr;
	mesh = nullptr;

	//SetScale(1.5f, 1.5f, 1.5f);
}

HexSelector::~HexSelector() {
	if (mesh != nullptr) {
		delete mesh;
	}
}

void HexSelector::Update() {

}

void HexSelector::Draw() {
	if (mesh != nullptr) {
		float size = target->planet->GetRadius() + 0.001f;

		mesh->Draw(
			World::GetShaderManager()->GetShader("default"),
			scale(vec3(size, size, size)),
			vec4(1, 1, 1, 1),
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