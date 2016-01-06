#include "Game.h"

#include "Test.h"

#include <iostream>

Game::Game() {
	scenes.push(new Test());
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
void Game::Update() {
	scenes.back()->Update();
}

void Game::Draw() {
	scenes.back()->Draw();
}