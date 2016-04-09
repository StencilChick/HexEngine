#pragma once

#include <glm/glm.hpp>
#include <stack>
#include "Scene.h"

#include "Galaxy.h"
#include "PlanetTypes.h"
#include "SphereManager.h"

class Game {
public:
	// singleton stuff
	static Game* GetInstance();
	static void Destroy();

	// stuff that makes the game run
	void Init();

	void Update();
	void Draw();

	// scenes
	static void PushScene(Scene*);
	static void PopScene();

	// manager
	static Galaxy* GetGalaxy();

	static PlanetTypeManager* GetPlanetTypeManager();
	static SphereManager* GetSphereManager();

private:
	// singleton stuff
	static Game *instance;

	Game();
	~Game();
	Game(Game const&);
	void operator=(Game const&);

	// scenes
	static std::stack<Scene*> scenes;

	static int popScene;
	static Scene *pushScene;

	// managers
	static Galaxy galaxy;

	static PlanetTypeManager planetTypeManager;
	static SphereManager sphereManager;
};