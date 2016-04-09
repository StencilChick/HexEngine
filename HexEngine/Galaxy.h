#pragma once

#include "Star.h"
#include "Timer.h"

class Galaxy {
public:
	Galaxy();
	Galaxy& operator=(Galaxy&);

	void Unload();

	void PopulateNew();
	void Load(const char*);

	void Update();
	void Draw();

	int GetSeed();

	int GetStarCount();
	Star* GetStar(int);

	Timer* GetTimer();

	const float starSize;

private:
	int seed;
	std::vector<Star> stars;

	Timer timer;

	void Populate();
};