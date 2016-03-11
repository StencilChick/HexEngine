#pragma once

#include <glm/glm.hpp>

struct Ray {
	glm::vec3 point;
	glm::vec3 direction;

	Ray(glm::vec3, glm::vec3);
};