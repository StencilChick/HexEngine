#include "GameHUD.h"

#include <iostream>

#include "World.h"
#include "Game.h"

GameHUD::GameHUD() {
	targetHex = nullptr;
	targetPlanet = nullptr;
	targetStar = nullptr;
}


void GameHUD::Update() {
	// timer controls
	if (Input::GetKeyDown(GLFW_KEY_SPACE)) {
		Timer *timer = Game::GetGalaxy()->GetTimer();

		if (timer->GetSpeed() == 0) {
			timer->SetSpeed(1);
		} else {
			timer->SetSpeed(0);
		}
	}
}

void GameHUD::Draw() {
	//DrawTimer();

	DrawTargetInfo();

	// seed
	World::GetFontManager()->WriteLine(
		World::GetFontManager()->GetAtlas("times.ttf"),
		("Seed #" + std::to_string(Game::GetGalaxy()->GetSeed())).c_str(),
		World::SCREEN_WIDTH-105, World::SCREEN_HEIGHT-25
		);
}


// functions for setting things
void GameHUD::SetTargetHex(PlanetHex *target) {
	targetHex = target;
	targetPlanet = nullptr;
	targetStar = nullptr;
}

void GameHUD::SetTargetPlanet(Planet *target) {
	targetHex = nullptr;
	targetPlanet = target;
	targetStar = nullptr;
}

void GameHUD::SetTargetStar(Star *target) {
	targetHex = nullptr;
	targetPlanet = nullptr;
	targetStar = target;
}

void GameHUD::ReleaseTarget() {
	targetHex = nullptr;
	targetPlanet = nullptr;
	targetStar = nullptr;
}


// functions for drawing things
void GameHUD::DrawTimer() {
	Timer *timer = Game::GetGalaxy()->GetTimer();
	World::GetFontManager()->WriteLine(
		World::GetFontManager()->GetAtlas("times.ttf"),
		(std::to_string(timer->GetDay()) + "/" + std::to_string(timer->GetMonth()) + "/" + std::to_string(timer->GetYear())).c_str(),
		5, 5
		);
}

void GameHUD::DrawTargetInfo() {
	if (targetHex != nullptr) {
		FontManager *fontMgr = World::GetFontManager();
		fontMgr->WriteLine(fontMgr->GetAtlas("times.ttf"), targetHex->GetBiome().c_str(), 5, World::SCREEN_HEIGHT-23);
	}
}