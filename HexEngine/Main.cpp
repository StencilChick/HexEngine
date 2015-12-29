#include "World.h"

int main() {
	World *world = World::GetInstance();
	world->SetUp();

	world->Run();

	World::Destroy();
}