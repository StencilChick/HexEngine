#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera;
class Game;

#include "Input.h"

#include "MeshManager.h"
#include "ShaderManager.h"
#include "ImageManager.h"

class World {
public:
	// singleton stuff
	static World* GetInstance();
	static void Destroy();

	// execution stuff
	void SetUp();
	void Run();

	// getting stuff
	static GLFWwindow* GetWindow();

	static MeshManager* GetMeshManager();
	static ShaderManager* GetShaderManager();
	static ImageManager* GetImageManager();

	const int SCREEN_WIDTH = 900;
	const int SCREEN_HEIGHT = 540;

private:
	// singleton stuff
	static World *instance;

	World();
	~World();
	World(World const&);
	void operator=(World const&);

	// gl stuff
	static GLFWwindow *window;

	// content stuff
	static MeshManager meshManager;
	static ShaderManager shaderManager;
	static ImageManager imageManager;

	Input *input;
	Camera *camera;

	Game *game;
};