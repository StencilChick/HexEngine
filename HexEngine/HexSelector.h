#pragma once
#include "GameObject.h"

#include "PlanetHelpers.h"

class HexSelector {
public:
	HexSelector();
	~HexSelector();

	void Update();
	void Draw();

	void SetTarget(PlanetHex*);
	void ReleaseTarget();
	PlanetHex* GetTarget();

private:
	PlanetHex *target;

	Mesh *mesh;
};