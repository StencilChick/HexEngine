#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Camera;
class Game;

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
	static MeshManager* GetMeshManager();
	static ShaderManager* GetShaderManager();
	static ImageManager* GetImageManager();

private:
	// singleton stuff
	static World *instance;

	World();
	~World();
	World(World const&);
	void operator=(World const&);

	// gl stuff
	GLFWwindow *window;

	// content stuff
	static MeshManager meshManager;
	static ShaderManager shaderManager;
	static ImageManager imageManager;

	Camera *camera;

	Game *game;
};