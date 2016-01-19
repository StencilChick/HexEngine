#pragma once

#include "PlanetHelpers.h"

#include <glm/glm.hpp>

#include <vector>

#include "Mesh.h"

class Planet {
public:
	Planet();
	~Planet();

	void SetUp(int);

	void Update();
	void Draw();

	void SetPos(glm::vec3);

private:
	std::vector<PlanetTri> tris;
	std::vector<PlanetHex> hexes;

	Mesh *mesh;

	glm::vec3 pos;
	int size;
};