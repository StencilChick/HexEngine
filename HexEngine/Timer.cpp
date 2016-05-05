#include "Timer.h"

#include <iostream>

#include "Input.h"
#include "World.h"

Timer::Timer() {
	time = 0;
}

void Timer::Update() {
	float lastTime = time;

	time += speed * Input::DeltaTime();

	if (time != lastTime) {
		//World::GetShaderManager()->UpdateTime(time);
	}
}


float Timer::GetTime() {
	return time;
}

void Timer::SetTime(float t) {
	time = t;
}


int Timer::GetSpeed() {
	return speed;
}

void Timer::SetSpeed(int s) {
	speed = s;
	if (speed > 5) speed = 5;
	else if (speed < 0) speed = 0;
}


int Timer::GetYear() {
	int val = time / 364;
	return val;
}

int Timer::GetMonth() {
	int val = time - GetYear()*364;
	return val / 28 + 1;
}

int Timer::GetDay() {
	int val = time - GetYear()*364 - (GetMonth()-1) * 28;
	return val + 1;
}