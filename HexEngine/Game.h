#pragma once

#include <glm/glm.hpp>
#include <queue>
#include "Scene.h"

#include "Galaxy.h"
#include "PlanetTypes.h"

class Game {
public:
	// singleton stuff
	static Game* GetInstance();
	static void Destroy();

	// stuff that makes the game run
	void SetUp();

	void Update();
	void Draw();

	// scenes
	void PushScene(Scene*);
	void PopScene();

	// manager
	static Galaxy* GetGalaxy();

	static PlanetTypeManager* GetPlanetTypeManager();

private:
	// singleton stuff
	static Game *instance;

	Game();
	~Game();
	Game(Game const&);
	void operator=(Game const&);

	// scenes
	std::queue<Scene*> scenes;

	int popScene;
	Scene *pushScene;

	// managers
	static Galaxy galaxy;

	static PlanetTypeManager planetTypeManager;
};