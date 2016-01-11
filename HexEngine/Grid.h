#pragma once

#include <vector>

#include "Hex.h"

// for testing
#include "Mesh.h"


class Grid {
public:
	Grid();
	~Grid();

	void SetUp(int, int, int);

	void Update();
	void Draw();

	vec2 GetLeftHex(int, int);
	vec2 GetRightHex(int, int);
	vec2 GetUpLeftHex(int, int);
	vec2 GetUpRightHex(int, int);
	vec2 GetDownLeftHex(int, int);
	vec2 GetDownRightHex(int, int);

	static float GetRawNoise();

private:
	std::vector<Hex> hexes;

	static int seed;
	int width;
	int height;

	vec2 CoordToHexPos(int, int);
	vec2 HexPosToCoord(float, float);

	// map gen functions
	static int noiseIndex;

	std::vector<float> GenLandMap();
	std::vector<float> GenTemperatureMap();
	std::vector<float> GenFoliageMap();

	// for testing
	Mesh *plane;
	GLuint texture;
};