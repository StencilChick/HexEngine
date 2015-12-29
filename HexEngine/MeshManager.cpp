#include "MeshManager.h"

#include "FileLoaders.h"

#include <iostream>
using namespace std;

MeshManager::MeshManager() {
	
}


// load them meshes
void MeshManager::Load() {
	meshes.clear();

	vector<string> files = ListFilesAtDirectory(".\\Data\\Meshes\\*.obj");
	
	int subPos;
	subPos = files[0].find("Meshes\\") + 7;

	for (int i = 0; i < files.size(); i++) {
		string name = files[i].substr(subPos);
		name = name.substr(0, name.size()-1);

		meshes[name] = new Mesh(("./Data/Meshes/" + name).c_str());
	}
}

void MeshManager::UnLoad() {
	for (map<string, Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		delete it->second;
	}
}

// get them meshes
Mesh* MeshManager::GetMesh(string name) {
	return meshes[name];
}
Mesh* MeshManager::GetMesh(const char* name) { return GetMesh(string(name)); }