#include "Scene.h"

#include "Generic.h"
#include "Grid.h"
#include "Hex.h"

class Test : public Scene {
public:
	Test();

	void Update();
	void Draw();

private:
	Grid grid;
};