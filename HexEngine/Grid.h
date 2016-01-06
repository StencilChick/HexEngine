#pragma once

#include <vector>

#include "Hex.h"

class Grid {
public:
	Grid();

	void SetUp(int, int);

	void Update();
	void Draw();

private:
	std::vector<Hex> hexes;

	vec2 CoordToHexPos(int, int);
	vec2 HexPosToCoord(float, float);
};