#pragma once

#include <vector>

#include <GL/glew.h>
#include <glm\glm.hpp>

class Planet;

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
	int height;
	int temp;

	float offset;

	std::vector<glm::vec3> triPos; // centres of all the triangles making this hex

	Planet *planet;

	// constructor
	PlanetHex(glm::vec3);

	void Assign(Planet*, int, int);

	// functions
	void AddHexToMesh(std::vector<GLfloat>&, std::vector<GLushort>&);
	void AddHexSurfaceToMesh(std::vector<GLfloat>&, std::vector<GLushort>&);

	static void CalcUV(int height, int temp, float &uvX, float &uvY);
};