#pragma once

#include "Scene.h"

#include "Planet.h"
#include "Galaxy.h"

#include "Hex.h"
#include "HexSelector.h"

class PlanetTest : public Scene {
public:
	PlanetTest();
	~PlanetTest();

	void Update();
	void Draw();

private:
	Galaxy galaxy;
	Planet planet;

	// camera controls
	float camRotX;
	float camRotY;

	float camZoom;

	// selector
	HexSelector selector;
};