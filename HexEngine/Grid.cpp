#include "Grid.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <iostream>

Grid::Grid() {

}

void Grid::SetUp(int width, int height) {
	hexes.clear();

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Hex h = Hex();

			vec2 hexPos = CoordToHexPos(x, y);
			h.SetPosition(vec3(hexPos.x, 0, hexPos.y));

			hexes.push_back(h);
		}
	}
}

// update
void Grid::Update() {

}

// draw
void Grid::Draw() {
	for (std::vector<Hex>::iterator it = hexes.begin(); it != hexes.end(); it++) {
		it->Draw();
	}
}


vec2 Grid::CoordToHexPos(int x, int y) {
	float xf = x;
	float yf = y;

	if (y % 2 == 0) {
		xf += 0.5f;
	}
	yf *= 0.75f;
	xf *= sin(M_PI/3);

	return vec2(xf, yf);
}