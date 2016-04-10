#pragma once

class GameHUD {
public:
	GameHUD();

	void Update();
	void Draw();

private:
	// functions for drawing things
	void DrawTimer();
};