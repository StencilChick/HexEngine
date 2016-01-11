#pragma once

#include <vector>

#include <glm/glm.hpp>

struct Pixel {
	float r;
	float g;
	float b;

	Pixel();
	Pixel(float, float, float);
};

struct Circle;


void DitherMap(int times, std::vector<float>&, int, int);
void BorderMap(int size, std::vector<float>&, int, int);
void BlurMap(int times, std::vector<float>&, int, int);
void BlurMapVertically(int times, std::vector<float>&, int, int);

void DrawMapCircle(int x, int y, int r, std::vector<float>&, int, int);
void DrawMapCircle(Circle c, std::vector<float>&, int, int);
void DrawMapCircle(int x, int y, int r, float val, std::vector<float>&, int, int);
void DrawMapCircle(Circle c, float val, std::vector<float>&, int, int);

void DrawMapSquare(int x, int y, int size, std::vector<float>&, int, int);

void DrawMapRect(int x, int y, int w, int h, float val, std::vector<float>&, int, int);


struct Circle {
	int x;
	int y;
	int r;

	Circle();
	Circle(int x, int y, int r);

	glm::vec2 GetPointOnEdge(float theta);
};