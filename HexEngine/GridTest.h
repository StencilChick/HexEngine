#include "Scene.h"

#include <glm/glm.hpp>

#include "Generic.h"
#include "Grid.h"
#include "Hex.h"

class GridTest : public Scene {
public:
	GridTest();

	void Update();
	void Draw();

private:
	Grid grid;
	int seed;

	glm::vec2 pos;
};