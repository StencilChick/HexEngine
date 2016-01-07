#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <map>
#include <vector>

#include "InstanceHelpers.h"

class ShaderManager {
public:
	ShaderManager();

	void Load();
	void UnLoad();

	int INSTANCE_LENGTH = 128;

	GLuint GetShader(std::string);
	GLuint GetShader(const char*);

	InstanceLoc GetInstanceLoc(GLuint, int);

	void UpdateViewMatrix(const glm::mat4&);
	void UpdateProjectionMatrix(const glm::mat4&);

private:
	std::map<std::string, GLuint> shaders;

	std::map<GLuint, std::vector<InstanceLoc>> shaderLocs;
};