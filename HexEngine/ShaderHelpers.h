#pragma once

#include <GL/glew.h>

#include <glm\glm.hpp>
using namespace glm;

char* LoadTextFile(const char* file);
GLuint LoadShader(const char* file, GLenum shaderType);
GLuint LoadShaderProgramme(const char* vertexFile, const char* fragmentFile);

GLint GetUniformLoc(GLuint, const char*);

void SetShaderBool(GLuint, GLint, bool);
void SetShaderBool(GLuint, const char*, bool);
void SetShaderInt(GLuint, GLint, int);
void SetShaderInt(GLuint, const char*, int);
void SetShaderFloat(GLuint, GLint, float);
void SetShaderFloat(GLuint, const char*, float);


void SetShaderV2(GLuint, GLint, const vec2&);
void SetShaderV2(GLuint, const char*, const vec2&);
void SetShaderV3(GLuint, GLint, const vec3&);
void SetShaderV3(GLuint, const char*, const vec3&);
void SetShaderV4(GLuint, GLint, const vec4&);
void SetShaderV4(GLuint, const char*, const vec4&);

void SetShaderM4(GLuint, GLint, const mat4&);
void SetShaderM4(GLuint, const char*, const mat4&);