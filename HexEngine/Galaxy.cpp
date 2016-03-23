#include "Galaxy.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <time.h>
#include <iostream>

#include <glm/gtx/transform.hpp>

#include "simplex\simplexnoise.h"

#include "World.h"

Galaxy::Galaxy() {
	
}


void Galaxy::PopulateNew() {
	srand(time(NULL));
	seed = rand() % 10000;

	std::cout << seed << std::endl;

	Populate();
}

void Galaxy::Load(const char *file) {

}


void Galaxy::Draw() {
	Mesh *mesh = World::GetMeshManager()->GetMesh("sphere.obj");

	for (std::vector<Star>::iterator it = stars.begin(); it != stars.end(); it++) {
		mesh->Draw(
			World::GetShaderManager()->GetShader("default"),
			glm::translate(it->GetPosition()) * glm::scale(glm::vec3(0.05f, 0.05f, 0.05f)),
			glm::vec4(1, 1, 1, 1),
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
	int starNum = 1000;

	float maxDist = 10;
	float minDist = 0.15f;
	float maxOffset = 0.65f;

	int armNum = 5; //5;
	float armBend = 5;

	for (int i = 0; i < starNum; i++) {
		// values
		float distVal = (octave_noise_2d(4, 0.15f, 1, i, seed) + 1) / 2;
		float distValSqr = pow(distVal, 2);

		float offset = maxOffset * (1 / distVal) * octave_noise_2d(4, 0.15f, 1, i, seed+1);
		float newOffset = pow(offset, 2);
		if (offset < 0) newOffset *= -1;
		offset = newOffset;

		float angle = (i % armNum) * M_PI*2 / armNum + offset + (distValSqr * armBend);
		float dist = minDist + (maxDist - minDist) * distValSqr;

		// star
		Star star = Star(
			this, 
			glm::vec3(
				cos(angle) * dist, 
				octave_noise_2d(4, 0.15f, 1, i, seed+2) * 0.15f,
				sin(angle) * dist
				)
			);
		stars.push_back(star);
	}
}