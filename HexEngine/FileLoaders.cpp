#include "FileLoaders.h"

#include <iostream>
#include <sstream>
#include <Windows.h>
using namespace std;

#include <array>
#include <map>

#include <glm/glm.hpp>
using namespace glm;

// lists files at a directory (and sub-directories)
vector<string> ListFilesAtDirectory(const char *path) {
	vector<string> value;

	FILE *pipe = NULL;
	string pCmd = "dir /B /S " + string(path);
	char buf[256];

	if (NULL == (pipe = _popen(pCmd.c_str(), "rt"))) {
		std::cout << "Error listing files at directory '" << path << "'\n";
	}

	while (!feof(pipe)) {
		if (fgets(buf, 256, pipe) != NULL) { value.push_back(string(buf)); }
	}

	_pclose(pipe);

	return value;
}

// read the contents of a file into a cstring
char* ReadFile(const char *path) {
	FILE *file;

	char *content = NULL;
	int length = 0;
	
	if (path != NULL) {
		fopen_s(&file, path, "rt") != NULL;

		if (file != NULL) {
			fseek(file, 0, SEEK_END);
			length = ftell(file);
			rewind(file);

			if (length > 0) {
				content = new char[length + 1];
				fread(content, sizeof(char), length, file);
				content[length] = '\0';
			}

			fclose(file);
		}
	}

	return content;
}

// reads an OBJ file into vertices and elements
void LoadOBJ(const char *fileName, vector<GLfloat> &vertices, vector<GLushort> &elements) {
	char *fileContent = ReadFile(fileName);

	// holders for the data before making the finalised vertices
	vector<vec3> positions;
	vector<vec3> normals;
	vector<vec2> uvs;
	map<int, array<float, 8>> finalVerts;
	positions.reserve(1024); uvs.reserve(1024);

	// go through the thing and do things based on the things
	char line[256];
	int lineCount = 0;
	int i = 0;
	vector<string> params; params.reserve(5);

	while (fileContent[i]) {
		if (fileContent[i] == '\n') {
			line[lineCount] = '\0';

			// line is finished; do things
			switch(line[0]) {
			case 'v':
				switch(line[1]) {
				case 't':
					// uv
					params = SplitString(line, ' ');
					uvs.push_back(vec2(stof(params[1]), stof(params[2])));
					break;

				case 'n':
					//normal
					params = SplitString(line, ' ');
					normals.push_back(vec3(stof(params[1]), stof(params[2]), stof(params[3])));
					break;

				default:
					// position
					params = SplitString(line, ' ');
					positions.push_back(vec3(stof(params[1]), stof(params[2]), stof(params[3])));
					break;
				}
				break;

			case 'f':
				// face
				params = SplitString(line, ' ');
				int posIndex[3];
				int normIndex[3];
				int uvIndex[3];
				for (int i = 1; i <=3; i++) {
					vector<string> t = SplitString(params[i], '/');

					posIndex[i-1] = stoi(t[0])-1;
					uvIndex[i-1] = stoi(t[1])-1;
					normIndex[i-1] = stoi(t[2])-1;
				}

				for (int i = 0; i < 3; i++) {
					vec3 pos = positions[posIndex[i]];
					vec3 norm = normals[normIndex[i]];
					vec2 uv = uvs[uvIndex[i]];

					array<float, 8> point = { pos.x, pos.y, pos.z, uv.x, uv.y, norm.x, norm.y, norm.z };

					if (finalVerts.size() < 3) {
						// first face being defined here
						//finalVerts.push_back(point);
						finalVerts[finalVerts.size()] = point;
						elements.push_back(finalVerts.size() - 1);
					} else {
						// check if point already exists
						//vector<array<float, 5>>::iterator it = find(finalVerts.begin(), finalVerts.end(), point);

						int foundPoint = -1;
						for (map<int, array<float, 8>>::iterator it = finalVerts.begin(); it != finalVerts.end(); it++) {
							if (it->second == point) { foundPoint = it->first; break; }
						}

						if (foundPoint > -1) {
							// found identical point
							elements.push_back(foundPoint);
						} else {
							// found unidentical point
							//finalVerts.push_back(point);
							finalVerts[finalVerts.size()] = point;
							elements.push_back(finalVerts.size() - 1);
						}
					}
				}
				break;
			}

			lineCount = 0;
		} else if (fileContent[i] == '\0') {
			// file is finshed; break
			break;
		} else {
			// part of line; add
			line[lineCount] = fileContent[i];
			lineCount++;
		}

		// iterate
		i++;
	}

	// push the finalised vertices to vertices
	cout << fileName << " - " << elements.size() << endl;

	for (int i = 0; i < finalVerts.size(); i++) {
		for (int ii = 0; ii < 8; ii++) {
			vertices.push_back(finalVerts[i][ii]);
		}
	}
}

// splits a string
vector<string> SplitString(string s, char d) {
	vector<string> value;

	istringstream ss(s);
	while (!ss.eof()) {
		string token;
		getline(ss, token, d);
		value.push_back(token);
	}

	return value;
}