#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm\glm.hpp>

struct PlanetTri {
	// points
	glm::vec3 points[3];
	glm::vec3 centre;

	// constructor
	PlanetTri();
	PlanetTri(glm::vec3, glm::vec3, glm::vec3);

	// functions
	void Subdivide(PlanetTri*);

	void AddTriToMesh(std::vector<GLfloat>&, std::vector<GLushort>&);
};

struct PlanetHex {
	// values
	glm::vec3 pos;

	std::vector<std::vector<PlanetTri>::iterator> tris;

	// constructor
	PlanetHex(glm::vec3);

	// functions
	void AddHexToMesh(std::vector<GLfloat>&, std::vector<GLushort>&);
};