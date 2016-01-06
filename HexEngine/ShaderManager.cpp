#include "ShaderManager.h"

#include "ShaderHelpers.h"
#include "FileLoaders.h"

#include <iostream>

ShaderManager::ShaderManager() {

}

// load 'em
void ShaderManager::Load() {
	shaders.clear();

	vector<string> files = ListFilesAtDirectory(".\\Data\\Shaders\\*.vert");
	int subPos = files[0].find("Shaders\\") + 8;

	for (int i = 0; i < files.size(); i++) {
		string name = files[i].substr(subPos);
		name = name.substr(0, name.size()-1);

		// load it
		string fore = "./Data/Shaders/" + name.substr(0, name.size()-5);
		
		GLuint p = LoadShaderProgramme((fore+".vert").c_str(), (fore+".frag").c_str());
		
		shaders[name.substr(0, name.size()-5)] = p;
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