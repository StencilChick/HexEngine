#pragma once

#include <glm/glm.hpp>

#include "Planet.h"
class Galaxy;

enum StarType {
	yellow,
	red,
	blue
};

class Star {
public:
	Star(Galaxy*, glm::vec3);

	void SetUp();

	glm::vec3 GetPosition();
	StarType GetType();
	glm::vec4 GetColour();

	int GetPlanetCount();
	Planet* GetPlanet(int);

private:
	glm::vec3 position;
	std::vector<Planet*> planets;

	StarType type;
	glm::vec4 colour;

	Galaxy *galaxy;
};