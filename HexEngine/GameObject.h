#pragma once

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
using namespace glm;

#include "Mesh.h"

class GameObject {
public:
	GameObject();
	GameObject(const char*, const char*);
	GameObject(const char*, const char*, const char*);

	// update-y draw-y stuff
	virtual void Update() = 0;
	virtual void Draw();

	// transform
	void Translate(vec3);
	void Tranlsate(float, float, float);

	void TranslateLocal(vec3);
	void TranslateLocal(float, float, float);

	void Rotate(float, vec3);
	void RotateLocal(float, vec3);

	void Scale(vec3);

	void SetPosition(vec3);
	void SetRotation(quat);
	void SetScale(vec3);

protected:
	// meshes and things
	Mesh *mesh;
	GLuint shader;
	GLuint texture;

	// transform values
	vec3 position;
	quat rotation;
	vec3 scale;
};