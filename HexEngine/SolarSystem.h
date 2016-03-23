#pragma once

#include "Scene.h"

#include "Star.h"

class SolarSystem : Scene {
public:
	SolarSystem(Star*);

	void Update();
	void Draw();

private:
	Star *star;
};