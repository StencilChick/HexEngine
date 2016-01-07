#pragma once

#include "Mesh.h"

#include <map>
#include <vector>
#include <string>

class MeshManager {
public:
	MeshManager();

	void Load();
	void UnLoad();

	void DrawMeshes();

	Mesh* GetMesh(std::string);
	Mesh* GetMesh(const char*);

	std::vector<std::string> GetMeshNames();

private:
	std::map<std::string, Mesh*> meshes;
	std::vector<std::string> meshNames;
};