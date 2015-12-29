#pragma once

#include "GameObject.h"

class Camera : public GameObject {
public:
	// singleton stuff
	static Camera* GetInstance();
	static void Destroy();

	void Update();

	static mat4 GetViewMatrix();

	static mat4 GetProjectionMatrix();
	static void SetProjectionMatrix(float fov, float aspect, float near, float far);

private:
	// singleton stuff
	static Camera *instance;

	Camera();
	~Camera();
	Camera(Camera const&);
	void operator=(Camera const&);

	// the view matrix
	static mat4 viewMatrix;
	static mat4 projectionMatrix;
};