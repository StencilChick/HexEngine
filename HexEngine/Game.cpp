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

	galaxy = Galaxy();
	galaxy.PopulateNew(); // for testing
	
	//scenes.push(new PlanetTest());
	scenes.push(new GalaxyView());
}

void Game::Update() {
	scenes.back()->Update();
}

void Game::Draw() {
	scenes.back()->Draw();
}


// manager
Galaxy* Game::GetGalaxy() {
	return &galaxy;
}

PlanetTypeManager* Game::GetPlanetTypeManager() {
	return &planetTypeManager;
}