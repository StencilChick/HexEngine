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
	static double DeltaTime();

	// mouse
	static glm::vec2 MousePos();
	static glm::vec2 MouseGLPos();
	static glm::vec2 MouseDelta();

	bool MouseButton(int);
	bool MouseButtonDown(int);
	bool MouseButtonUp(int);

	float MouseScroll();

	void BindCursor();
	void UnbindCursor();
	static bool IsCursorBound();

	// keys
	static bool GetKey(int);
	static bool GetKeyDown(int);
	static bool GetKeyUp(int);

private:
	// singleton stuff
	static Input *instance;

	Input();
	~Input();
	Input(Input const&);
	Input operator=(Input const&);


	// value stuff
	static double lastTime;
	static double thisTime;

	static glm::vec2 lastMousePos;
	static glm::vec2 thisMousePos;

	bool lastMouseButtons[3];
	bool theseMouseButtons[3];

	static float mouseScrollDelta;

	static bool mouseScrollThisFrame;
	static void MouseWheelCallback(GLFWwindow *window, double xOff, double yOff);


	static std::map<int, bool> theseKeys;
	static std::map<int, bool> lastKeys;

	static bool keyPressedThisFrame;
	static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);
};