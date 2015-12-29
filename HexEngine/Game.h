#pragma once

#include <glm/glm.hpp>

#include "Generic.h"

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

	// for testing
	//Mesh *m;
	Generic g;
};