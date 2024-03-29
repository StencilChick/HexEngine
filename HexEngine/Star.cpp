#include "Star.h"

#include "simplex\simplexnoise.h"

#include <algorithm>
#include <iostream>

#include "Galaxy.h"

Star::Star(Galaxy *galaxy, glm::vec3 position, int starType) {
	this->galaxy = galaxy;

	this->position = position;

	type = static_cast<StarType>(starType);
}
Star::~Star() {
	
}

void Star::Unload() {
	for (std::vector<Planet*>::iterator it = planets.begin(); it != planets.end(); it++) {
		delete *it;
	}
	planets.clear();
}

void Star::Init() {
	// star
	switch (type) {
	case StarType::blue:
		colour = glm::vec4(0.75f, 0.75f, 1, 1);
		break;

	case StarType::white:
		colour = glm::vec4(0.97f, 0.97f, 1, 1);
		break;

	case StarType::yellow:
		colour = glm::vec4(1, 1, 0.5f, 1);
		break;

	case StarType::orange:
		colour = glm::vec4(1, 0.5f, 0.15f, 1);
		break;

	case StarType::red:
		colour = glm::vec4(1, 0.15f, 0.15f, 1);
		break;

	default:
		colour = glm::vec4(1, 1, 1, 1);
		break;
	}

	
	// planets
	int numPlanets = 1 + (raw_noise_4d(position.x, position.y, position.z, galaxy->GetSeed() + 2) + 1)/2 * 8;

	frostLine = numPlanets - (raw_noise_4d(position.x, position.y, position.z, galaxy->GetSeed() + 1) + 1)/2 * std::min(4, numPlanets/2+1);
	if (frostLine < 1) frostLine = 1;

	for (int i = 1; i <= numPlanets; i++) {
		Planet *p = new Planet(this);
		p->Init(nullptr, i);

		planets.push_back(p);
	}
}


glm::vec3 Star::GetPosition() {
	return position;
}

StarType Star::GetType() {
	return type;
}

glm::vec4 Star::GetColour() {
	return colour;
}


int Star::GetPlanetCount() {
	return planets.size();
}

Planet* Star::GetPlanet(int i) {
	return planets[i];
}

int Star::GetFrostLine() {
	return frostLine;
}