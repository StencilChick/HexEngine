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
	void Translate(float, float, float);

	void TranslateLocal(vec3);
	void TranslateLocal(float, float, float);

	void Rotate(float, vec3);
	void RotateLocal(float, vec3);

	void Scale(vec3);

	void SetPosition(vec3);
	void SetPosition(float, float, float);

	void SetRotation(quat);
	void SetRotation(float, vec3);

	void SetScale(vec3);
	void SetScale(float, float, float);

	vec3 GetPosition();
	quat GetRotation();
	vec3 GetScale();

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