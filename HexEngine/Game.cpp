#include "Game.h"

#include "PlanetTest.h"
#include "SolarSystem.h"
#include "GalaxyView.h"

#include <iostream>

Galaxy Game::galaxy;
PlanetTypeManager Game::planetTypeManager;

Game::Game() {
	
}

Game::~Game() {
	while (scenes.size() > 0) {
		delete scenes.back();
		scenes.pop();
	}
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
void Game::SetUp() {
	planetTypeManager = PlanetTypeManager();
	planetTypeManager.Load();

	popScene = 0;
	pushScene = nullptr;

	galaxy = Galaxy();
	galaxy.PopulateNew(); // for testing
	
	//scenes.push(new PlanetTest());
	//scenes.push(new GalaxyView());
	scenes.push(new SolarSystem(galaxy.GetStar(0)));
}

void Game::Update() {
	// push/pop scenes
	while (popScene > 0) {
		delete scenes.back();
		scenes.pop();

		popScene--;
	}
	if (pushScene != nullptr) {
		scenes.push(pushScene);

		pushScene = nullptr;
	}

	// update top scene
	scenes.back()->Update();
}

void Game::Draw() {
	scenes.back()->Draw();
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