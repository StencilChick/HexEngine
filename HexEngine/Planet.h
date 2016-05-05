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

	void Init(Planet*, int);

	void SetUpMesh();
	void DeleteMesh();

	void Update();
	void Draw();

	void DrawOrbit(float);


	int GetSizeVal();
	bool IsMoon();
	bool IsGasPlanet();


	int GetMoonCount();
	Planet* GetMoon(int);

	
	int GetDistance();

	glm::vec3 GetPosition();
	float GetRadius();
	float GetRotation();

	PlanetType* GetType();

	bool GetRayHit(glm::vec3, glm::vec3, glm::vec3&);
	bool GetRayHit(Ray, glm::vec3&);
	bool GetRayHit(glm::vec3, glm::vec3);
	bool GetRayHit(Ray);

	PlanetHex* GetClosestHexToPos(glm::vec3);

	float GetDistanceToOrbitCentre();

private:
	Star *star;
	Planet *parent; // if this planet is a moon

	bool isGasPlanet;

	std::vector<Planet*> moons;

	std::vector<Mesh*> meshes;
	bool meshCreated;

	int radius;
	int distance;
	float startAngle;
	float revolveTime;
	float rotateTime;

	int size;
	PlanetType *type;

	float seaLevel;


	void BuildTerrestianPlanet(glm::vec3);
	void BuildGasPlanet(glm::vec3);

	int GetHexHeight(glm::vec3);
	int GetHexTemp(glm::vec3);
};