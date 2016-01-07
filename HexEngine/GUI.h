#pragma once

#include <glm/glm.hpp>

#include "FontHelpers.h"

class GUI {
public:
	// singleton stuff
	static GUI* GetInstance();
	static void Destroy();

	// screen coordinate conversion stuff
	static glm::vec2 ScreenPosToGL(glm::vec2);
	static glm::vec2 ScreenPosToGL(int, int);

	static glm::vec2 ScreenSizeToGL(glm::vec2);
	static glm::vec2 ScreenSizeToGL(int, int);

	// image/text depth
	static float GetDepth();
	void ResetDepth();

private:
	// singleton stuff
	static GUI *instance;

	GUI();
	~GUI();
	GUI(GUI const&);
	void operator=(GUI const&);

	// values
	static float depth;
};