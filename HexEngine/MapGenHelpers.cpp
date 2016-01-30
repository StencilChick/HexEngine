#include "MapGenHelpers.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "Grid.h"

#include <iostream>

// pixel
Pixel::Pixel() { }
Pixel::Pixel(float r, float g, float b) {
	this->r = r;
	this->g = g;
	this->b = b;
}


// map functions
void DitherMap(int times, std::vector<float> &map, int w, int h) {
	std::vector<float> newMap;
	newMap.resize(map.size());

	if (times > 1) {
		DitherMap(times-1, map, w, h);
	}

	for (int x = 0; x < w; x++) {
		int left = x - 1; if (left < 0) left = w - 1;
		int right = x + 1; if (right >= w) right = 0;

		for (int y = 0; y < h; y++) {
			int up = y - 1; if (up < 0) up = h - 1;
			int down = y + 1; if (down >= h) down = 0;

			std::vector<float> vals;

			vals.push_back(map[y*w+left]);
			vals.push_back(map[y*w+right]);
			vals.push_back(map[down*w+x]);
			vals.push_back(map[down*w+x]);
			
			newMap[y*w+x] = vals[Grid::GetRawNoise() * 4];
		}
	}

	map = newMap;
}

void BorderMap(int size, std::vector<float> &map, int w, int h) {
	if (size > 1) {
		BorderMap(size-1, map, w, h);
	}

	std::vector<float> newMap;
	newMap.resize(map.size());

	for (int x = 0; x < w; x++) {
		int left = x - 1; if (left < 0) left = w - 1;
		int right = x + 1; if (right >= w) right = 0;

		for (int y = 0; y < h; y++) {
			if (map[y*w+x] != 0) {
				newMap[y*w+x] = map[y*w+x];

				continue;
			}

			int up = y - 1; if (up < 0) up = h - 1;
			int down = y + 1; if (down >= h) down = 0;

			if (map[y*w+left] != 0) {
				newMap[y*w+x] = 1;
			} else if (map[y*w+right] != 0) {
				newMap[y*w+x] = 1;
			} else if (map[up*w+x] != 0) {
				newMap[y*w+x] = 1;
			} else if (map[down*w+x] != 0) {
				newMap[y*w+x] = 1;
			}
		}
	}

	map = newMap;
}

void BlurMap(int times, std::vector<float> &map, int w, int h) {
	if (times > 1) {
		BlurMap(times-1, map, w, h);
	}

	std::vector<float> newMap;
	newMap.resize(map.size());

	for (int x = 0; x < w; x++) {
		int left = x - 1; if (left < 0) left += w;
		int right = x + 1; if (right >= w) right -= w;

		for (int y = 0; y < h; y++) {
			int up = y - 1; if (up < 0) up += h;
			int down = y + 1; if (down >= h) down -= h;

			newMap[y*w+x] = (map[up*w+x] + map[down*w+x] + map[y*w+right] + map[y*w+left]) / 4;
		}
	}

	map = newMap;
}

void BlurMapVertically(int times, std::vector<float> &map, int w, int h) {
	if (times > 1) {
		BlurMapVertically(times-1, map, w, h);
	}

	std::vector<float> newMap;
	newMap.resize(map.size());

	for (int y = 0; y < h; y++) {
		int up = y - 1; if (up < 0) up += h;
		int down = y + 1; if (down >= h) down -= h;

		for (int x = 0; x < w; x++) {
			newMap[y*w+x] = (map[up*w+x] + map[down*w+x]) / 2;
		}
	}

	map = newMap;
}

// draw things on the map
void DrawMapCircle(int x, int y, int r, float val, std::vector<float> &pixels, int w, int h) {
	while(x < 0) x += w;
	while(x >= w) x -= w;

	while(y < 0) y += h;
	while(y >= h) y -= h;

	for (int i = 0; i < r; i++) {
		int up = y + i; while (up >= h) up -= h;
		int down = y - i; while (down < 0) down += h;

		for (int j = 0; j < ceil(cos(M_PI/2 * i/r) * r); j++) {
			int right = x + j; while (right >= w) right -= w;
			int left = x - j; while (left < 0) left += w;

			//std::cout << i << " " << j << " : " << up << " " << down << " " << left << " " << right << " : " << (up*w+right) << " " << (up*w+left) << " " << (down*w+right) << " " << (down*w+left) << std::endl;

			pixels[up * w + right] = val;
			pixels[up * w + left] = val;
			pixels[down * w + right] = val;
			pixels[down * w + left] = val;
		}
	}
}
void DrawMapCircle(int x, int y, int r, std::vector<float> &map, int w, int h) { DrawMapCircle(x, y, r, 1, map, w, h); }
void DrawMapCircle(Circle c, std::vector<float> &pixels, int w, int h) { DrawMapCircle(c.x, c.y, c.r, pixels, w, h); }
void DrawMapCircle(Circle c, float val, std::vector<float>& map, int w, int h) { DrawMapCircle(c.x, c.y, c.r, val, map, w, h); }

void DrawMapSquare(int x, int y, int size, std::vector<float> &pixels, int w, int h) {
	for (int i = x - size; i < x + size; i++) {
		int xPos = i;
		if (xPos < 0) xPos += w;
		else if (xPos >= w) xPos -= w;

		for (int ii = y - size; ii < y + size; ii++) {
			int yPos = ii;
			if (yPos < 0) yPos += h;
			else if (yPos >= h) yPos -= h;

			pixels[yPos * w + xPos] = 1;
		}
	}
}

void DrawMapRect(int x, int y, int width, int height, float val, std::vector<float> &map, int w, int h) {
	for (int i = 0; i < width; i++) {
		int xPos = x + i;
		if (xPos < 0) xPos += w;
		else if (xPos >= w) xPos -= w;

		for (int ii = 0; ii < height; ii++) {
			int yPos = y + ii;
			if (yPos < 0) yPos += h;
			else if (yPos >= h) yPos -= h;

			map[yPos * w + xPos] = val;
		}
	}
}


// shape junk
Circle::Circle() { }
Circle::Circle(int x, int y, int r) {
	this->x = x;
	this->y = y;
	this->r = r;
}

glm::vec2 Circle::GetPointOnEdge(float theta) {
	return glm::vec2(
		cos(theta) * r + x,
		sin(theta) * r + y
		);
}


// map junk
MapBlock::MapBlock() { }
MapBlock::MapBlock(bool land, bool vegetation, float temperature) {
	this->land = land;
	this->vegetation = vegetation;
	this->temperature = temperature;

	if (land) {
		if (temperature >= 0.66f) {
			vegetation ? biome = Biome::amazon : biome = Biome::desert;
		} else if (temperature >= 0.33f) {
			vegetation ? biome = Biome::temperate : biome = Biome::plains;
		} else {
			vegetation ? biome = Biome::taiga : biome = Biome::polar;
		}
	} else {
		biome = Biome::ocean;
	}
}

vec3 MapBlock::GetColour() {
	switch(biome) {
	case Biome::polar:
		return vec3(0.7f, 0.7f, 0.7f);

	case Biome::taiga:
		return vec3(0.1f, 0.5f, 0.4f);

	case Biome::plains:
		return vec3(0.7f, 0.8f, 0.1f);

	case Biome::temperate:
		return vec3(0.1f, 0.9f, 0.1f);

	case Biome::desert:
		return vec3(0.9f, 0.8f, 0.8f);

	case Biome::amazon:
		return vec3(0.1f, 0.5f, 0.1f);

	case Biome::ocean:
		return vec3(0, 0, 0.4f);

	default:
		return vec3(0, 0, 0);
	}
}