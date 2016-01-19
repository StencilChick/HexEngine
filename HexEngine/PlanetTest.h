#pragma once

#include "Scene.h"

#include "Planet.h"
#include "Hex.h"
#include "Generic.h"

class PlanetTest : public Scene {
public:
	PlanetTest();
	~PlanetTest();

	void Update();
	void Draw();

private:
	Planet planet;
};