#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct InstanceData {
	glm::mat4 modelMatrix;
	glm::vec4 colour;

	int imageID;

	InstanceData(const glm::mat4 &modelMatrix, const glm::vec4 &colour, int imageID) {
		this->modelMatrix = modelMatrix;
		this->colour = colour;
		this->imageID = imageID;
	}
};

struct InstanceLoc {
	GLint modelLoc;
	GLint colourLoc;
	GLint texIdLoc;

	GLint imageLoc;

	InstanceLoc() {

	}
	InstanceLoc(GLint modelLoc, GLint colourLoc, GLint texIdLoc, GLint imageLoc) {
		this->modelLoc = modelLoc;
		this->colourLoc = colourLoc;
		this->texIdLoc = texIdLoc;

		this->imageLoc = imageLoc;
	}
};