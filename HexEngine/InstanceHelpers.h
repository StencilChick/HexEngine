#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

struct InstanceData {
	glm::mat4 modelMatrix;
	glm::vec4 colour;

	InstanceData(const glm::mat4 &modelMatrix, const glm::vec4 &colour) {
		this->modelMatrix = modelMatrix;
		this->colour = colour;
	}
};

struct InstanceLoc {
	GLint modelLoc;
	GLint colourLoc;

	InstanceLoc() {

	}
	InstanceLoc(GLint modelLoc, GLint colourLoc) {
		this->modelLoc = modelLoc;
		this->colourLoc = colourLoc;
	}
};