#include "Galaxy.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <time.h>
#include <iostream>

#include <glm/gtx/transform.hpp>

#include "simplex\simplexnoise.h"

#include "World.h"

Galaxy::Galaxy() : starSize(0.05f) {
	timer = Timer();
}

Galaxy& Galaxy::operator=(Galaxy &other) {
	if (&other == this) {
		return *this;
	}

	this->seed = other.GetSeed();
	this->timer = *other.GetTimer();

	stars.clear();
	for (int i = 0; i < other.GetStarCount(); i++) {
		stars.push_back(*other.GetStar(i));
	}

	return *this;
}

void Galaxy::Unload() {
	for (std::vector<Star>::iterator it = stars.begin(); it != stars.end(); it++) {
		it->Unload();
	}
}


void Galaxy::PopulateNew() {
	srand(time(NULL));
	seed = rand() % 10000;

	timer.SetTime(0);

	Populate();
}

void Galaxy::Load(const char *file) {
	timer.SetTime(0);
}


void Galaxy::Update() {
	timer.Update();
}

void Galaxy::Draw() {
	Mesh *mesh = World::GetMeshManager()->GetMesh("sphere.obj");

	for (std::vector<Star>::iterator it = stars.begin(); it != stars.end(); it++) {
		mesh->Draw(
			World::GetShaderManager()->GetShader("default"),
			glm::translate(it->GetPosition()) * glm::scale(glm::vec3(starSize, starSize, starSize)),
			it->GetColour(),
			World::GetImageManager()->GetImage("white.png")
			);
	}
}


int Galaxy::GetSeed() {
	return seed;
}

int Galaxy::GetStarCount() {
	return stars.size();
}

Star* Galaxy::GetStar(int index) {
	return &stars[index];
}


void Galaxy::Populate() {
	int starNum = 1000; //1000;

	float maxDist = 7.5f;
	float minDist = 0.25f;
	float maxOffset = 0.65f;

	float starDist = 0.15f;

	int armNum = 4;
	float armBend = 5;

	int i = 0;
	while (stars.size() < starNum) {
		// values
		float distVal = (raw_noise_2d(i, seed) + 1) / 2;
		float distValSqr = pow(distVal, 2);

		float offset = maxOffset * (1 / distVal) * raw_noise_2d(i, seed+1);
		float newOffset = pow(offset, 2);
		if (offset < 0) newOffset *= -1;
		offset = newOffset;

		float angle = (i % armNum) * M_PI*2 / armNum + offset + (distValSqr * armBend);
		float dist = minDist + (maxDist - minDist) * distValSqr;

		glm::vec3 position = glm::vec3(
			cos(angle) * dist, 
			raw_noise_2d(i, seed+2) * 0.15f * (1 - distValSqr),
			sin(angle) * dist
			);

		i++;

		// check
		bool good = true;
		for (std::vector<Star>::iterator it = stars.begin(); it != stars.end(); it++) {
			if (glm::length(glm::vec2(it->GetPosition().x, it->GetPosition().z) - glm::vec2(position.x, position.z)) < starDist) {
				good = false;
				break;
			}
		}
		if (!good) continue;

		// star
		stars.push_back(Star(this, position));
		stars.back().Init();
	}
}


Timer* Galaxy::GetTimer() {
	return &timer;
}