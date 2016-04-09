#pragma once

class Timer {
public:
	Timer();

	void Update();

	float GetTime();
	void SetTime(float);

	int GetSpeed();
	void SetSpeed(int);

	int GetDay();
	int GetMonth();
	int GetYear();

private:
	float time;
	int speed;
};