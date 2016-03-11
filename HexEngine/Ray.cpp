#include "Ray.h"

Ray::Ray(glm::vec3 point, glm::vec3 direction) {
	this->point = point;
	this->direction = direction;
}