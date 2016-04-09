#include "Game.h"

#include "PlanetTest.h"
#include "SolarSystem.h"
#include "GalaxyView.h"

#include <iostream>

std::stack<Scene*> Game::scenes;
int Game::popScene;
Scene* Game::pushScene;

Galaxy Game::galaxy;
PlanetTypeManager Game::planetTypeManager;
SphereManager Game::sphereManager;

Game::Game() {
	
}

Game::~Game() {
	while (scenes.size() > 0) {
		std::cout << scenes.top() << std::endl;

		delete scenes.top();
		scenes.pop();
	}

	galaxy.Unload();
}

// singleton stuff
Game* Game::instance = nullptr;
Game* Game::GetInstance() {
	if (instance == nullptr) {
		instance = new Game();
	}

	return instance;
}
void Game::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}


// draw and update
void Game::Init() {
	planetTypeManager = PlanetTypeManager();
	planetTypeManager.Load();

	sphereManager = SphereManager();
	sphereManager.Init();

	popScene = 0;
	pushScene = nullptr;

	galaxy = Galaxy();
	galaxy.PopulateNew(); // for testing
	
	//scenes.push(new PlanetTest());
	scenes.push(new GalaxyView());
	scenes.push(new SolarSystem(galaxy.GetStar(0)));
}

void Game::Update() {
	// push/pop scenes
	while (popScene > 0) {
		delete scenes.top();
		scenes.pop();

		popScene--;
	}
	if (pushScene != nullptr) {
		scenes.push(pushScene);

		pushScene = nullptr;
	}

	// update top scene
	scenes.top()->Update();
}

void Game::Draw() {
	scenes.top()->Draw();
}


// scenes
void Game::PushScene(Scene *scene) {
	pushScene = scene;
}

void Game::PopScene() {
	popScene++;
}


// manager
Galaxy* Game::GetGalaxy() {
	return &galaxy;
}

PlanetTypeManager* Game::GetPlanetTypeManager() {
	return &planetTypeManager;
}

SphereManager* Game::GetSphereManager() {
	return &sphereManager;
}