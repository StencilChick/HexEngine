#pragma once

#include "PlanetHelpers.h"
#include "Planet.h"
#include "Star.h"

class GameHUD {
public:
	GameHUD();

	void Update();
	void Draw();

	void SetTargetHex(PlanetHex*);
	void SetTargetPlanet(Planet*);
	void SetTargetStar(Star*);
	void ReleaseTarget();

private:
	PlanetHex *targetHex;
	Planet *targetPlanet;
	Star *targetStar;

	// functions for drawing things
	void DrawTimer();
	void DrawTargetInfo();
};