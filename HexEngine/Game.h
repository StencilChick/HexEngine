#pragma once

#include <glm/glm.hpp>
#include <queue>
#include "Scene.h"

class Game {
public:
	// singleton stuff
	static Game* GetInstance();
	static void Destroy();

	// stuff that makes the game run
	void Update();
	void Draw();

private:
	// singleton stuff
	static Game *instance;

	Game();
	~Game();
	Game(Game const&);
	void operator=(Game const&);

	// scenes
	std::queue<Scene*> scenes;
};