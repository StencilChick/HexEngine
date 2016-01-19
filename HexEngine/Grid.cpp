#include "Grid.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include "World.h"
#include <glm/gtx/transform.hpp>

#include "simplex\simplexnoise.h"
#include "MapGenHelpers.h"

#include <time.h>
#include <iostream>

int Grid::seed = 0;
int Grid::noiseIndex = 0;

Grid::Grid() {
	plane = nullptr;
}

Grid::~Grid() {
	if (plane != nullptr) {
		delete plane;
		glDeleteTextures(1, &texture);
	}
}

void Grid::SetUp(int seed, int width, int height) {
	this->seed = seed;
	this->width = width;
	this->height = height;

	noiseIndex = 0;
	worldMap.resize(width * height);

	std::vector<float> land = GenLandMap();
	std::vector<float> temp = GenTemperatureMap();
	std::vector<float> fol = GenFoliageMap();

	// for testing
	plane = new Mesh("./Data/Meshes/plane.obj");

	blockPos = vec2(-1, -1);

	// convert pixel vector to float vector
	std::vector<float> pixels;
	pixels.resize(land.size() * 3);

	for (int i = 0; i < land.size(); i++) {
		MapBlock block = MapBlock(land[i], fol[i], temp[i]);
		worldMap[i] = block;
		
		vec3 colour = block.GetColour();
		pixels[i*3] = colour.x;
		pixels[i*3+1] = colour.y;
		pixels[i*3+2] = colour.z;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, &pixels[0]);

	glBindTexture(GL_TEXTURE_2D, 0);
}

// update
void Grid::Update() {
	
}

// draw
void Grid::Draw() {
	for (std::vector<Hex>::iterator it = hexes.begin(); it != hexes.end(); it++) {
		it->Draw();
	}

	glBindTexture(GL_TEXTURE_2D, texture);

	plane->Draw(World::GetShaderManager()->GetShader("default"), translate(vec3(0, 0, 0)) * scale(vec3(1.28f, 1, 0.80f)), vec4(1, 1, 1, 1));
	//plane->Draw(World::GetShaderManager()->GetShader("default"), mat4(), vec4(1, 1, 1, 1));
	plane->BindBuffersAndDraw();
}


// map generation stuff
float Grid::GetRawNoise() {
	noiseIndex++;
	return min((raw_noise_2d(noiseIndex, seed) + 1) / 2, 0.999f);
}

std::vector<float> Grid::GenLandMap() {
	std::vector<float> values;
	values.resize(width * height);

	// throw some shapes all up in this
	std::vector<Circle> circles;

	int maxRad = (width + height) / 16;
	for (int i = maxRad; i >= maxRad / 4; i /= 2) {
		// put some circles in there
		for (int ii = 0; ii < maxRad / i * 2; ii++) {
			Circle c = Circle(GetRawNoise() * width, GetRawNoise() * height, i);

			DrawMapCircle(c, values, width, height);
			circles.push_back(c);
		}

		// put some squares in there
		for (int ii = 0; ii < maxRad / 3; ii++) {
			int index = ii; while (index >= circles.size()) index -= circles.size();

			vec2 pos = circles[index].GetPointOnEdge(M_PI*2 * GetRawNoise());
			DrawMapSquare((int)pos.x, (int)pos.y, maxRad / 6, values, width, height);
		}
	}

	vec2 pos = circles[circles.size() * GetRawNoise()].GetPointOnEdge(2*M_PI * GetRawNoise());
	DrawMapCircle(pos.x, pos.y, maxRad, values, width, height);

	// dither and buffer
	DitherMap(6, values, width, height);
	BorderMap(1, values, width, height);

	// return the end result
	return values;
}

std::vector<float> Grid::GenTemperatureMap() {
	std::vector<float> values;
	values.resize(width * height);

	// draw a banner through the map
	DrawMapRect(0, 2*height/8, width, height/8, 0.5f, values, width, height);
	DrawMapRect(0, height/8*5, width, height/8, 0.5f, values, width, height);

	// put some circles through there
	std::vector<Circle> circles;
	circles.resize(3);
	for (int i = 0; i < 3; i++) {
		Circle c = Circle(
			width/3 * i - width/6 + 2*width/6*GetRawNoise(), 
			height/2 - height/8 + 2*height/8*GetRawNoise(), 
			height/2.5f);
		circles[i] = c;
		DrawMapCircle(c, 0.5f, values, width, height);
	}
	for (int i = 0; i < 3; i++) {
		circles[i].r = height/6;
		DrawMapCircle(circles[i], values, width, height);
	}

	// finish the banner
	DrawMapRect(0, height/2 - height/8, width, height/4, 1, values, width, height);

	// dither it up
	DitherMap(5, values, width, height);

	// return the end result
	return values;
}

std::vector<float> Grid::GenFoliageMap() {
	std::vector<float> values;
	values.resize(width * height);

	// make some circles up in this
	std::vector<Circle> circles;
	circles.resize(8);
	for (int i = 0; i < 8; i++) {
		Circle c = Circle(GetRawNoise() * width, GetRawNoise() * height, height/8);
		circles[i] = c;
		DrawMapCircle(c, values, width, height);
	}
	for (int i = 0; i < 16; i++) {
		vec2 pos = circles[GetRawNoise()*8].GetPointOnEdge(M_PI*2 * GetRawNoise());
		DrawMapCircle(pos.x, pos.y, height/16, values, width, height);
	}

	// dither it up
	BorderMap(8, values, width, height);
	DitherMap(4, values, width, height);

	// return the thing
	return values;
}


// width and height
int Grid::GetWidth() {
	return width;
}

int Grid::GetHeight() {
	return height;
}

// cartesian coordinates to a hex position
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


// find coordinates of adjacent hexes
vec2 Grid::GetLeftHex(int x, int y) {
	x--;
	if (x < 0) x += width;

	return vec2(x, y);
}

vec2 Grid::GetRightHex(int x, int y) {
	x++;
	if (x >= width) x -= width;

	return vec2(x, y);
}

vec2 Grid::GetUpLeftHex(int x, int y) {
	y++;
	if (x % 2 == 1) {
		x--;
		if (x < 0) x += width;
	}

	return vec2(x, y);
}

vec2 Grid::GetUpRightHex(int x, int y) {
	y++;
	if (x % 2 == 0) {
		x++;
		if (x >= width) x -= width;
	}

	return vec2(x, y);
}

vec2 Grid::GetDownLeftHex(int x, int y) {
	y--;
	if (x % 2 == 1) {
		x--;
		if (x < 0) x += width;
	}

	return vec2(x, y);
}

vec2 Grid::GetDownRightHex(int x, int y) {
	y--;
	if (x % 2 == 0) {
		x++;
		if (x >= width) x -= width;
	}

	return vec2(x, y);
}


// testing function
void Grid::SetBlockPos(vec2 pos) {
	if (pos != blockPos) {
		blockPos = pos;

		vec3 colour = worldMap[pos.y*width + pos.x].GetColour();
		float pixels[] = {
			colour.x, colour.y, colour.z
		};

		// image
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 1, 1, 0, GL_RGB, GL_FLOAT, pixels);
	}
}