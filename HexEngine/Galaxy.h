#pragma once

#include "Star.h"

class Galaxy {
public:
	Galaxy();

	void PopulateNew();
	void Load(const char*);

	void Draw();

	int GetSeed();

	int GetStarCount();
	Star* GetStar(int);

private:
	int seed;

	std::vector<Star> stars;

	void Populate();
};