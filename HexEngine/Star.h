#pragma once

#include <glm/glm.hpp>

#include "Planet.h"
class Galaxy;

enum StarType {
	blue,
	white,
	yellow,
	orange,
	red
};

class Star {
public:
	Star(Galaxy*, glm::vec3, int);
	~Star();

	void Init();
	void Unload();

	glm::vec3 GetPosition();
	StarType GetType();
	glm::vec4 GetColour();

	int GetPlanetCount();
	Planet* GetPlanet(int);

	int GetFrostLine();

private:
	glm::vec3 position;
	std::vector<Planet*> planets;

	StarType type;
	glm::vec4 colour;

	int frostLine;

	Galaxy *galaxy;
};