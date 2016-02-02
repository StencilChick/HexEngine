#include "GameObject.h"

#include <GL/glew.h>
#include <glm/gtx/transform.hpp>

#include "ShaderHelpers.h"
#include "Camera.h"

#include "World.h"

#include <iostream>

GameObject::GameObject() {
	position = vec3(0, 0, 0);
	rotation = quat();
	scale = vec3(1, 1, 1);

	colour = vec4(1, 1, 1, 1);
}

GameObject::GameObject(const char *modelName, const char *texName) : GameObject() {
	this->mesh = World::GetMeshManager()->GetMesh(modelName);
	this->texture = World::GetImageManager()->GetImage(texName);

	this->shader = World::GetShaderManager()->GetShader("default");
}

GameObject::GameObject(const char *modelName, const char *shaderName, const char *texName) : GameObject() {
	this->mesh = World::GetMeshManager()->GetMesh(modelName);
	this->shader = World::GetShaderManager()->GetShader(shaderName);
	this->texture = World::GetImageManager()->GetImage(texName);
}

void GameObject::Draw() {
	mesh->Draw(shader, translate(position) * glm::scale(scale) * mat4_cast(rotation), colour, texture);
}

// all the transform junk (uuuuugh... so looooong...)
void GameObject::Translate(vec3 v) {
	position += v;
}
void GameObject::Translate(float x, float y, float z) { Translate(vec3(x, y, z)); }

void GameObject::TranslateLocal(vec3 v) {
	v = vec3(rotation * vec4(v, 0));
	position += v;
}
void GameObject::TranslateLocal(float x, float y, float z) { TranslateLocal(vec3(x, y, z)); }


void GameObject::Rotate(float a, vec3 v) {
	quat q = inverse(quat(cos(a/2), v.x * sin(a/2), v.y * sin(a/2), v.z * sin(a/2)));
	rotation = normalize(q * rotation);
}

void GameObject::RotateLocal(float a, vec3 v) {
	quat q = inverse(quat(cos(a/2), v.x * sin(a/2), v.y * sin(a/2), v.z * sin(a/2)));
	rotation = normalize(rotation * q);
}


void GameObject::Scale(vec3 v) {
	scale *= v;
}


void GameObject::SetPosition(vec3 v) {
	position = v;
}
void GameObject::SetPosition(float x, float y, float z) { SetPosition(vec3(x, y, z)); }

void GameObject::SetRotation(quat q) {
	rotation = q;
}
void GameObject::SetRotation(float a, vec3 v) { SetRotation(inverse(quat(cos(a/2), v.x * sin(a/2), v.y * sin(a/2), v.z * sin(a/2)))); }

void GameObject::SetScale(vec3 v) {
	scale = v;
}
void GameObject::SetScale(float x, float y, float z) { SetScale(vec3(x, y, z)); }


vec3 GameObject::GetPosition() {
	return position;
}

quat GameObject::GetRotation() {
	return rotation;
}

vec3 GameObject::GetScale() {
	return scale;
}


// colour
vec4 GameObject::GetColour() {
	return colour;
}

void GameObject::SetColour(const vec4 &v) {
	colour = v;
}
void GameObject::SetColour(float r, float g, float b, float a) { SetColour(vec4(r, g, b, a)); }