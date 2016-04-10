#include "GameHUD.h"

#include "World.h"
#include "Game.h"

GameHUD::GameHUD() {
	
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
	DrawTimer();
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