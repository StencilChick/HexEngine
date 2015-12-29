#pragma once

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <map>

class ShaderManager {
public:
	ShaderManager();

	void Load();
	void UnLoad();

	GLuint GetShader(std::string);
	GLuint GetShader(const char*);

	void UpdateViewMatrix(const glm::mat4&);
	void UpdateProjectionMatrix(const glm::mat4&);

private:
	std::map<std::string, GLuint> shaders;
};