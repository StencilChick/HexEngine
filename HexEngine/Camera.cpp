#include "Camera.h"

#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/gtx/transform.hpp>

#include "World.h"

// some values
mat4 Camera::viewMatrix = mat4();
mat4 Camera::projectionMatrix = mat4();

mat4 Camera::inverseViewMatrix = mat4();
mat4 Camera::inverseProjectionMatrix = mat4();

// constructor
Camera::Camera() {

}

Camera::~Camera() {

}

// singleton stuff
Camera* Camera::instance = nullptr;
Camera* Camera::GetInstance() {
	if (instance == nullptr) {
		instance = new Camera();
	}
	return instance;
}

void Camera::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}


// updates the view matrix
void Camera::Update() {
	inverseViewMatrix = glm::translate(position) * mat4_cast(rotation);
	viewMatrix = inverse(inverseViewMatrix);

	World::GetShaderManager()->UpdateViewMatrix(viewMatrix);
}

// sets the projection matrix
void Camera::SetProjectionMatrix(float fov, float aspect, float near, float far) {
	float top = near * tan((M_PI/180) * (fov/2));
	float bottom = -top;
	float right = top * aspect;
	float left = -right;

	projectionMatrix = mat4(
		(2*near)/(right-left), 0, (right+left)/(right-left), 0,
		0, (2*near)/(top-bottom), (top+bottom)/(top-bottom), 0,
		0, 0, -(far+near)/(far-near), -(2*far*near)/(far-near),
		0, 0, -1, 0
		);
	projectionMatrix = transpose(projectionMatrix); // because glm can't just have its matrix axis order be convenient.  no, that would be too easy!

	inverseProjectionMatrix = inverse(projectionMatrix);

	World::GetShaderManager()->UpdateProjectionMatrix(projectionMatrix);
}


// return the matrices
mat4 Camera::GetViewMatrix() {
	return viewMatrix;
}

mat4 Camera::GetProjectionMatrix() {
	return projectionMatrix;
}


mat4 Camera::GetInverseViewMatrix() {
	return inverseViewMatrix;
}

mat4 Camera::GetInverseProjectionMatrix() {
	return inverseProjectionMatrix;
}