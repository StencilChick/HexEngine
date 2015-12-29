#pragma once

#include <GL/glew.h>

class Mesh {
public:
	Mesh();
	Mesh(GLfloat*, int, GLushort*, int);
	Mesh(const char*);

	~Mesh();

	void Draw();

private:
	GLuint buffer;
	GLuint elements;
	GLuint vao;

	void BindGL(GLfloat*, int, GLushort*, int);
};