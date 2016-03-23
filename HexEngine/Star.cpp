#include "Star.h"

#include "Galaxy.h"

Star::Star(Galaxy *galaxy, glm::vec3 position) {
	this->galaxy = galaxy;

	this->position = position;
}

void Star::SetUp() {

}


glm::vec3 Star::GetPosition() {
	return position;
}