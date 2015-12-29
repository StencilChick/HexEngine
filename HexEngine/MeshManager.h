#pragma once

#include "Mesh.h"

#include <map>
#include <string>

class MeshManager {
public:
	MeshManager();

	void Load();
	void UnLoad();

	Mesh* GetMesh(std::string);
	Mesh* GetMesh(const char*);

private:
	std::map<std::string, Mesh*> meshes;
};