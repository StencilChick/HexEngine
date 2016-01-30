#include "Cursor.h"

#include "World.h"

// singleton stuff
Cursor* Cursor::instance = nullptr;

Cursor* Cursor::GetInstance() {
	if (instance == nullptr) {
		instance = new Cursor();
	}

	return instance;
}

void Cursor::Destroy() {
	if (instance != nullptr) {
		delete instance;
		instance = nullptr;
	}
}

// constructor
Cursor::Cursor() {

}

Cursor::~Cursor() {

}

