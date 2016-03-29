#pragma once

#include "PlanetHelpers.h"
#include "Ray.h"

#include <glm/glm.hpp>

#include <vector>

#include "Mesh.h"
#include "PlanetTypes.h"

class Star;

class Planet {
public:
	Planet();
	Planet(Star*);
	~Planet();

	void SetUp(int, int);
	void SetUpMesh();
	void DeleteMesh();

	void Update();
	void Draw();

	float GetRadius();
	int GetDistance();

	PlanetType* GetType();

	bool GetRayHit(glm::vec3, glm::vec3, glm::vec3&);
	bool GetRayHit(Ray, glm::vec3&);

	PlanetHex* GetClosestHexToPos(glm::vec3);

private:
	Star *star;

	std::vector<PlanetTri> tris;
	std::vector<PlanetHex> hexes;

	std::vector<Mesh*> meshes;
	bool meshCreated;

	int radius;
	int distance;
	float startAngle;

	int size;
	PlanetType *type;
};