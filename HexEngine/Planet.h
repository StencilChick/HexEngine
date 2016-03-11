#pragma once

#include "PlanetHelpers.h"
#include "Ray.h"

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
	glm::vec3 GetPos();
	float GetRadius();

	bool GetRayHit(glm::vec3, glm::vec3, glm::vec3&);
	bool GetRayHit(Ray, glm::vec3&);

	PlanetHex* GetClosestHexToPos(glm::vec3);

private:
	std::vector<PlanetTri> tris;
	std::vector<PlanetHex> hexes;

	Mesh *mesh;

	glm::vec3 pos;
	int radius;

	int size;
};