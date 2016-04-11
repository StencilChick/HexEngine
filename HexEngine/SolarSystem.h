#pragma once

#include "Scene.h"

#include "Star.h"

class SolarSystem : public Scene {
public:
	SolarSystem(Star*);
	~SolarSystem();

	void Update();
	void Draw();

private:
	Star *star;

	// modes
	enum Mode {
		solar,
		planet
	};
	Mode mode;

	void SwitchToPlanetMode();
	void SwitchToSolarMode();

	// camera
	float camDist;
	float camRotX;
	float camRotY;
	glm::vec3 camFocus;

	float maxDistPlanet;
	float minDistSolar;
	float maxDistSolar;

	Planet *targetPlanet;


	// functions
	void TargetLoop();

	void UpdateCamSolar();
	void UpdateCamPlanet();

	void UpdateControlsSolar();
	void UpdateControlsPlanet();
};