#include "PlanetHelpers.h"

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

#include <glm/gtc/quaternion.hpp>

#include <iostream>

// planet triangle
PlanetTri::PlanetTri() { }
PlanetTri::PlanetTri(glm::vec3 a, glm::vec3 b, glm::vec3 c) {
	points[0] = glm::normalize(a);
	points[1] = glm::normalize(b);
	points[2] = glm::normalize(c);

	centre = (a+b+c)/3.0f; //glm::normalize((a + b + c) / 3.0f);
}


void PlanetTri::Subdivide(PlanetTri *ref) {
	glm::vec3 ab = (points[0] + points[1]) / 2.0f;
	glm::vec3 bc = (points[1] + points[2]) / 2.0f;
	glm::vec3 ca = (points[2] + points[0]) / 2.0f;

	ref[0] = PlanetTri(points[0], ab, ca);
	ref[1] = PlanetTri(points[1], bc, ab);
	ref[2] = PlanetTri(points[2], ca, bc);
	ref[3] = PlanetTri(ab, bc, ca);
}

void PlanetTri::AddTriToMesh(std::vector<GLfloat> &verts, std::vector<GLushort> &elems) {
	// vertices
	verts.push_back(points[0].x);
	verts.push_back(points[0].y);
	verts.push_back(points[0].z);
	verts.push_back(0.5f);
	verts.push_back(0.95f);

	verts.push_back(points[1].x);
	verts.push_back(points[1].y);
	verts.push_back(points[1].z);
	verts.push_back(1 - 0.11f);
	verts.push_back(0.27f);

	verts.push_back(points[2].x);
	verts.push_back(points[2].y);
	verts.push_back(points[2].z);
	verts.push_back(0.11f);
	verts.push_back(0.27f);

	// elements
	int i = elems.size();
	
	elems.push_back(i);
	elems.push_back(i+1);
	elems.push_back(i+2);
}


// planet hex
PlanetHex::PlanetHex(glm::vec3 pos) {
	this->pos = pos;
}

void PlanetHex::AddHexToMesh(std::vector<GLfloat> &vertices, std::vector<GLushort> &elements) {
	std::vector<glm::vec3> points;
	glm::vec3 centre = glm::vec3(0, 0, 0);

	for (std::vector<std::vector<PlanetTri>::iterator>::iterator it = tris.begin(); it != tris.end(); it++) {
		points.push_back((*it)->centre);

		centre += (*it)->centre;
	}
	centre /= points.size();

	// vertices
	float uvX = (rand() % 100) * 1.0f/100, uvY = (rand() % 100) * 1.0f/100;

	glm::vec3 realCentre = glm::normalize(centre) * (0.935617f + height);
	vertices.push_back(realCentre.x);
	vertices.push_back(realCentre.y);
	vertices.push_back(realCentre.z);
	vertices.push_back(uvX);
	vertices.push_back(uvY);

	glm::vec3 len = points[1] - centre;
	float angleDelta = M_PI / points.size();
	glm::vec3 axis = glm::normalize(centre);

	for (int i = 0; i < points.size(); i++) {
		float angle = angleDelta * i;

		glm::vec3 newPoint = centre + len * glm::quat(cos(angle), axis.x * sin(angle), axis.y * sin(angle), axis.z * sin(angle));
		for (std::vector<glm::vec3>::iterator it = points.begin(); it != points.end(); it++) {
			if (abs(glm::length(*it - newPoint)) < 0.1f) {
				newPoint = glm::normalize(*it) * (1.0f + height);
				break;
			}
		}

		vertices.push_back(newPoint.x);
		vertices.push_back(newPoint.y);
		vertices.push_back(newPoint.z);
		vertices.push_back(uvX);
		vertices.push_back(uvY);

		newPoint = glm::normalize(newPoint) * 0.5f;

		vertices.push_back(newPoint.x);
		vertices.push_back(newPoint.y);
		vertices.push_back(newPoint.z);
		vertices.push_back(uvX);
		vertices.push_back(uvY);
	}

	// elements
	int index = vertices.size()/5 - (1+points.size()*2);
	for (int i = 0; i < points.size(); i++) {
		int point = 1 + i*2 + index;
		int back = 2 + i*2 + index;
		int nextPoint = 3 + i*2 + index; if (nextPoint > index-1+points.size()*2) nextPoint = index + 1;
		int nextBack = 4 + i*2 + index; if (nextBack > index+points.size()*2) nextBack = index + 2;

		elements.push_back(index);
		elements.push_back(nextPoint);
		elements.push_back(point);

		elements.push_back(point);
		elements.push_back(nextPoint);
		elements.push_back(back);

		elements.push_back(nextPoint);
		elements.push_back(nextBack);
		elements.push_back(back);
	}
}