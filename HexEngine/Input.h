#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <map>

class Input {
public:
	// singleton stuff
	static Input* GetInstance();
	static void Destory();


	// other stuff
	void Update();

	// time
	double DeltaTime();

	// mouse
	glm::vec2 MousePos();
	glm::vec2 MouseGLPos();
	glm::vec2 MouseDelta();

	bool MouseButton(int);
	bool MouseButtonDown(int);
	bool MouseButtonUp(int);

	// keys
	bool GetKey(int);
	bool GetKeyDown(int);
	bool GetKeyUp(int);

private:
	// singleton stuff
	static Input *instance;

	Input();
	~Input();
	Input(Input const&);
	Input operator=(Input const&);


	// value stuff
	double lastTime;
	double thisTime;

	glm::vec2 lastMousePos;
	glm::vec2 thisMousePos;

	bool lastMouseButtons[3];
	bool theseMouseButtons[3];

	static std::map<int, bool> theseKeys;
	static std::map<int, bool> lastKeys;

	static bool keyPressedThisFrame;

	static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};