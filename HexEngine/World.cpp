#include "World.h"

#include <iostream>

#include "Camera.h"
#include "Game.h"

MeshManager World::meshManager;
ShaderManager World::shaderManager;
ImageManager World::imageManager;

World::World() {

}
World::~World() {
	Game::Destroy();

	shaderManager.UnLoad();
	meshManager.UnLoad();
}

// singleton stuff
World* World::instance = nullptr;
World* World::GetInstance() {
	if (instance == nullptr) {
		instance = new World();
	}

	return instance;
}

void World::Destroy() {
	if (instance != nullptr) {
		delete instance;
	}
	instance = nullptr;
}

// setting up all the openGL and stuff
void World::SetUp() {
	// GL
	if (!glfwInit()) {
		std::cout << "GLFW problem\n";
		std::cin.get();
		return;
	}

	glfwWindowHint(GLFW_RESIZABLE, 0);
	window = glfwCreateWindow(900, 540, "Hex Engine", NULL, NULL);
	glfwMakeContextCurrent(window);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK) { 
		std::cout << "GLEW problem\n";
		std::cin.get();
		return;
	}

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// stuff
	meshManager = MeshManager();
	meshManager.Load();
	shaderManager = ShaderManager();
	shaderManager.Load();
	imageManager = ImageManager();
	imageManager.Load();

	camera = Camera::GetInstance();
	Camera::SetProjectionMatrix(45, 900.0f/540, 0.1f, 100);

	game = Game::GetInstance();
}

// running all the junk and stuff
void World::Run() {
	while (!glfwWindowShouldClose(window)) {
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glClearColor(0.392f, 0.584f, 0.929f, 1);

		camera->Update();

		game->Update();
		game->Draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return;
}


// for getting stuff
MeshManager* World::GetMeshManager() {
	return &meshManager;
}

ShaderManager* World::GetShaderManager() {
	return &shaderManager;
}

ImageManager* World::GetImageManager() {
	return &imageManager;
}