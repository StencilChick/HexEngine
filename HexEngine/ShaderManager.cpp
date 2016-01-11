#include "ShaderManager.h"

#include "ShaderHelpers.h"
#include "FileLoaders.h"

#include "World.h"
#include "MeshManager.h"

#include <string>
#include <iostream>

ShaderManager::ShaderManager() {

}

// load 'em
void ShaderManager::Load() {
	shaders.clear();

	MeshManager *meshMgr = World::GetMeshManager();

	vector<string> files = ListFilesAtDirectory(".\\Data\\Shaders\\*.vert");
	int subPos = files[0].find("Shaders\\") + 8;

	for (int i = 0; i < files.size(); i++) {
		string name = files[i].substr(subPos);
		name = name.substr(0, name.size()-1);

		// load it
		string fore = "./Data/Shaders/" + name.substr(0, name.size()-5);
		
		GLuint p = LoadShaderProgramme((fore+".vert").c_str(), (fore+".frag").c_str());
		shaders[name.substr(0, name.size()-5)] = p;

		SetShaderBool(p, "textured", true);

		// reference it
		vector<string> meshNames = meshMgr->GetMeshNames();
		for (int i = 0; i < meshNames.size(); i++) {
			meshMgr->GetMesh(meshNames[i])->AddShader(p);
		}

		// get shader locations... it
		vector<InstanceLoc> v;
		v.resize(INSTANCE_LENGTH);
		for (int i = 0; i < INSTANCE_LENGTH; i++) {
			v[i] = InstanceLoc(
				GetUniformLoc(p, ("modelMatrix[" + to_string(i) + "]").c_str()),
				GetUniformLoc(p, ("colour[" + to_string(i) + "]").c_str())
				);
		}
		shaderLocs[p] = v;
	}
}

// unload 'em
void ShaderManager::UnLoad() {
	for (map<string, GLuint>::iterator it = shaders.begin(); it != shaders.end(); it++) {
		glDeleteProgram(it->second);
	}
}


// get 'em
GLuint ShaderManager::GetShader(string name) {
	return shaders[name];
}
GLuint ShaderManager::GetShader(const char* name) { return shaders[string(name)]; }

InstanceLoc ShaderManager::GetInstanceLoc(GLuint shader, int index) {
	return shaderLocs[shader][index];
}


// update 'em
void ShaderManager::UpdateProjectionMatrix(const mat4 &m) {
	for (map<string, GLuint>::iterator it = shaders.begin(); it != shaders.end(); it++) {
		SetShaderM4(it->second, "projectionMatrix", m);
	}
}

void ShaderManager::UpdateViewMatrix(const mat4 &m) {
	for (map<string, GLuint>::iterator it = shaders.begin(); it != shaders.end(); it++ ) {
		SetShaderM4(it->second, "viewMatrix", m);
	}
}