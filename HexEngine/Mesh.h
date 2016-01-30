#pragma once

#include <map>
#include <vector>

#include <GL/glew.h>

#include "InstanceHelpers.h"

#include "Image.h"

class Mesh {
public:
	Mesh();
	Mesh(GLfloat*, int, GLushort*, int);
	Mesh(const char*);

	~Mesh();

	void Draw(GLuint, const glm::mat4&, const glm::vec4&, Image*);
	void BindBuffersAndDraw();

	void AddShader(GLuint);

private:
	GLuint buffer;
	GLuint elements;
	GLuint vao;

	void BindGL(GLfloat*, int, GLushort*, int);

	std::map<GLuint, std::vector<InstanceData>> instanceData;

	std::vector<Image*> images;
	int imgWidth;
	int imgHeight;
};