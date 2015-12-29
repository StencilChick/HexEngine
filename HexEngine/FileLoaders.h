#pragma once

#include <GL/glew.h>

#include <vector>
using namespace std;

// find files at a directory
vector<string> ListFilesAtDirectory(const char *path);

// read a file
char* ReadFile(const char *path);
// returns vertices and elements from an obj file
void LoadOBJ(const char *fileName, vector<GLfloat> &vertices, vector<GLushort> &elems);

// a thing
vector<string> SplitString(string s, char d);