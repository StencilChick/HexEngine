#pragma once

#include "Scene.h"

#include "Galaxy.h"

class GalaxyView : public Scene {
public:
	GalaxyView();

	void Update();
	void Draw();

private:
	Galaxy *galaxy;

	float camDist;
	float camRotX;
	float camRotY;
};