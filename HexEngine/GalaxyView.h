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

	Star* GetRayhitStar(Ray);

	void GoToSystemView();

	// camera
	Star *targetStar;

	float camDist;
	float camRotX;
	float camRotY;
	glm::vec3 camFocus;

	void UpdateCamera();
};