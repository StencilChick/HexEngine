#include "ShaderHelpers.h"

#include <GL/glew.h>

#include <fstream>
#include <iostream>
using namespace std;

#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
using namespace glm;

char* LoadTextFile(const char* filename) {
	ifstream infile(filename, ios::binary);
	if (infile.is_open()) {
		infile.seekg(0, ios::end);
		int length = (int)infile.tellg();
		infile.seekg(0, ios::beg);

		char *data = new char[length+1];
		infile.read(data, length);
		data[length] = 0;
		infile.close();

		return data;
	}

	return 0;
}

GLuint LoadShader(const char* filename, GLenum shaderType) {
	const char *file = LoadTextFile(filename);
	if (file == 0) { cout << "Everything is awful!\n"; return 0; }

	GLuint shader = glCreateShader(shaderType);
	glShaderSource(shader, 1, &file, 0);
	glCompileShader(shader);

	delete[] file;

	GLint result;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &result);
	if (result) {
		return shader;
	} else {
		GLint length;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetShaderInfoLog(shader, length, 0, log);

		cout << "Shader Error(" << filename << "): " << endl << log;

		delete log;
		return 0;
	}
}

GLuint LoadShaderProgramme(const char* vertexFile, const char* fragmentFile) {
	GLuint vertexShader = LoadShader(vertexFile, GL_VERTEX_SHADER);
	if (vertexShader == 0) { return 0; }
	GLuint fragmentShader = LoadShader(fragmentFile, GL_FRAGMENT_SHADER);
	if (fragmentShader == 0) { return 0; }

	GLuint program = glCreateProgram();
	glAttachShader(program, vertexShader);
	glAttachShader(program, fragmentShader);
	glLinkProgram(program);

	GLint result;
	glGetProgramiv(program, GL_LINK_STATUS, &result);
	if (result) {
		return program;
	} else {
		GLint length;
		glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
		char *log = new char[length];
		glGetProgramInfoLog(program, length, 0, log);

		cout << "Program Error: " << log;

		delete[] log;
		return 0;
	}
}


// getting functions
GLint GetUniformLoc(GLuint programme, const char *name) {
	return glGetUniformLocation(programme, name);
}


// all the setting things functions
void SetShaderBool(GLuint programme, GLint loc, bool value) {
	glUseProgram(programme);
	glUniform1i(loc, value);
}
void SetShaderBool(GLuint programme, const char *name, bool value) { SetShaderBool(programme, GetUniformLoc(programme, name), value); }

void SetShaderInt(GLuint programme, GLint loc, int value) {
	glUseProgram(programme);
	glUniform1i(loc, value);
}
void SetShaderInt(GLuint programme, const char *name, int value) { SetShaderInt(programme, GetUniformLoc(programme, name), value); }

void SetShaderFloat(GLuint programme, GLint loc, float value) {
	glUseProgram(programme);
	glUniform1f(loc, value);
}
void SetShaderFloat(GLuint programme, const char *name, float value) { SetShaderFloat(programme, GetUniformLoc(programme, name), value); }

void SetShaderV2(GLuint programme, GLint loc, const vec2 &value) {
	glUseProgram(programme);
	glUniform2f(loc, value.x, value.y);
}
void SetShaderV2(GLuint programme, const char *name, const vec2 &value) { SetShaderV2(programme, GetUniformLoc(programme, name), value); }

void SetShaderV3(GLuint programme, GLint loc, const vec3 &value) {
	glUseProgram(programme);
	glUniform4f(loc, value.x, value.y, value.z, 1);
}
void SetShaderV3(GLuint programme, const char *name, const vec3 &value) { SetShaderV3(programme, GetUniformLoc(programme, name), value); }
void SetShaderV4(GLuint programme, GLint loc, const vec4 &value) {
	glUseProgram(programme);
	glUniform4f(loc, value.x, value.y, value.z, value.w);
}
void SetShaderV4(GLuint programme, const char*name, const vec4 &value) { SetShaderV4(programme, GetUniformLoc(programme, name), value); }


void SetShaderM4(GLuint programme, GLint loc, const mat4 &matrix) {
	glUseProgram(programme);
	glUniformMatrix4fv(loc, 1, false, &matrix[0][0]);
}
void SetShaderM4(GLuint programme, const char *name, const mat4 &matrix) { SetShaderM4(programme, GetUniformLoc(programme, name), matrix); }