#pragma once

#include <glm/glm.hpp>

#include "Planet.h"
class Galaxy;

enum StarType {
	yellow
};

class Star {
public:
	Star(Galaxy*, glm::vec3);

	void SetUp();

	glm::vec3 GetPosition();
	StarType GetType();

private:
	glm::vec3 position;
	std::vector<Planet> planets;

	StarType type;

	Galaxy *galaxy;
};