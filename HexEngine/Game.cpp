#include "Game.h"

#include "GridTest.h"
#include "PlanetTest.h"

#include <iostream>

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
	
	scenes.push(new PlanetTest());
}

void Game::Update() {
	scenes.back()->Update();
}

void Game::Draw() {
	scenes.back()->Draw();
}


// manager
PlanetTypeManager* Game::GetPlanetTypeManager() {
	return &planetTypeManager;
}