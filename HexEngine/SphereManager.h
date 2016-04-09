#pragma once

#include <map>
#include <vector>

#include "PlanetHelpers.h"

class SphereManager {
public:
	SphereManager();

	void Init();

	const std::vector<PlanetHex>& GetHexSphere(int);

private:
	std::map<int, std::vector<PlanetHex>> hexSpheres;
};