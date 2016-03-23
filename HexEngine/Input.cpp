#include "Input.h"

#include "World.h"

#include <iostream>
using namespace glm;

// values
double Input::thisTime = 0;
double Input::lastTime = 0;

glm::vec2 Input::lastMousePos;
glm::vec2 Input::thisMousePos;

float Input::mouseScrollDelta = 0;

bool Input::mouseScrollThisFrame = false;

std::map<int, bool> Input::theseKeys;
std::map<int, bool> Input::lastKeys;

bool Input::keyPressedThisFrame = false;

// constructor
Input::Input() {
	theseMouseButtons[0] = false;
	theseMouseButtons[1] = false;
	theseMouseButtons[2] = false;

	lastMousePos = vec2(0, 0);
	thisMousePos = vec2(0, 0);

	glfwSetInputMode(World::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	glfwSetKeyCallback(World::GetWindow(), KeyCallback);
	glfwSetScrollCallback(World::GetWindow(), MouseWheelCallback);
}

Input::~Input() {

}

// singleton stuff
Input* Input::instance = nullptr;
Input* Input::GetInstance() {
	if (instance == nullptr) {
		instance = new Input();
	}
	return instance;
}

void Input::Destory() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}


// update the things
void Input::Update() {
	GLFWwindow *window = World::GetWindow();
	
	// time
	lastTime = thisTime;
	thisTime = glfwGetTime();

	// mouse
	lastMouseButtons[0] = theseMouseButtons[0];
	lastMouseButtons[1] = theseMouseButtons[1];
	lastMouseButtons[2] = theseMouseButtons[2];
	theseMouseButtons[0] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS;
	theseMouseButtons[1] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS;
	theseMouseButtons[2] = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS;

	lastMousePos = thisMousePos;
	
	double x, y;
	glfwGetCursorPos(window, &x, &y);
	thisMousePos = vec2(x, y);

	if (!mouseScrollThisFrame) {
		mouseScrollDelta = 0;
	}
	mouseScrollThisFrame = false;

	// keys
	if (!keyPressedThisFrame) {
		lastKeys = theseKeys;
	}
	keyPressedThisFrame = false;
}


// get delta time
double Input::DeltaTime() {
	return thisTime - lastTime;
}


// get mouse pos
vec2 Input::MousePos() {
	return thisMousePos;
}

vec2 Input::MouseGLPos() {
	return GUI::ScreenPosToGL(thisMousePos);
}

vec2 Input::MouseDelta() {
	return thisMousePos - lastMousePos;
}

// mouse buttons
bool Input::MouseButton(int b) {
	return theseMouseButtons[b];
}

bool Input::MouseButtonDown(int b) {
	return theseMouseButtons[b] && !lastMouseButtons[b];
}

bool Input::MouseButtonUp(int b) {
	return !theseMouseButtons[b] && lastMouseButtons[b];
}

// mouse wheel
float Input::MouseScroll() {
	return mouseScrollDelta;
}

void Input::MouseWheelCallback(GLFWwindow *window, double xOff, double yOff) {
	mouseScrollThisFrame = true;

	mouseScrollDelta = (float)yOff;
}

// mouse moving around and junk
void Input::BindCursor() {
	glfwSetInputMode(World::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
}

void Input::UnbindCursor() {
	glfwSetInputMode(World::GetWindow(), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}

bool Input::IsCursorBound() {
	return glfwGetInputMode(World::GetWindow(), GLFW_CURSOR) == GLFW_CURSOR_DISABLED;
}


// keys
void Input::KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods) {
	lastKeys = theseKeys;
	theseKeys[key] = action != GLFW_RELEASE;

	keyPressedThisFrame = true;
}


bool Input::GetKey(int k) {
	if (theseKeys.find(k) == theseKeys.end()) {
		return false;
	}

	return theseKeys[k];
}

// update these later
bool Input::GetKeyDown(int k) {
	if (theseKeys.find(k) == theseKeys.end()) {
		return false;
	}
	
	if (theseKeys[k]) {
		if (lastKeys.find(k) == lastKeys.end()) {
			return theseKeys[k];
		}

		return theseKeys[k] && !lastKeys[k];
	}

	return false;
}

bool Input::GetKeyUp(int k) {
	if (theseKeys.find(k) == theseKeys.end()) {
		return false;
	}

	if (!theseKeys[k]) {
		/*if (lastKeys.find(k) == lastKeys.end()) {
			return false;
		}*/

		return !theseKeys[k] && lastKeys[k];
	}

	return false;
}