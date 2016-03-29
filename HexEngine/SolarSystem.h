#pragma once

#include "Scene.h"

#include "Star.h"

class SolarSystem : public Scene {
public:
	SolarSystem(Star*);

	void Update();
	void Draw();

private:
	Star *star;

	// camera
	float camDist;
	float camRotX;
	float camRotY;
	glm::vec3 camFocus;
};