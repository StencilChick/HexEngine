#include "Scene.h"

#include "Generic.h"

class Test : public Scene {
public:
	Test();

	void Update();
	void Draw();

private:
	Generic g;
};