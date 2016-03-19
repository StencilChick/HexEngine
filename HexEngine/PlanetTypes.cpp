#include "PlanetTypes.h"

#include "FileLoaders.h"

#include <iostream>

// type
PlanetType::PlanetType() {

}

PlanetType::PlanetType(const char *filename) {
	char *content = ReadFile(filename);

	char line[256];
	int lineCount = 0;
	int i = 0;

	while (content[i]) {
		if (content[i] == '\n') {
			// line finished
			line[lineCount] = '\0';
			lineCount = 0;

			ParseLine(line);
		} else if (content[i] == '\0') {
			// file finished
			line[lineCount] = '\0';

			ParseLine(line);

			break;
		} else {
			// add character to line
			line[lineCount] = content[i];
			lineCount++;
		}

		i++;
	}
}

void PlanetType::ParseLine(const char *line) {
	if (line[0] == '\0') { return; }
	else if (line[0] == '#') { return; }

	vector<string> parameters = SplitString(line, ':');

	if (parameters[0] == string("name")) {
		name = parameters[1];
	} else if (parameters[0] == string("desc")) {
		description = parameters[1];
	} else if (parameters[0] == string("image")) {
		image = "Planets\\" + parameters[1];
		//image = "testGradient.png";
	} else if (parameters[0] == string("hex")) {
		hexes[stoi(parameters[1])][stoi(parameters[2])] = parameters[3];
	} else if (parameters[0] == string("sea")) {
		minSeaLevel = stof(parameters[1]);
		maxSeaLevel = stof(parameters[2]);
	}
}

// manager
PlanetTypeManager::PlanetTypeManager() {
	
}

void PlanetTypeManager::Load() {
	types.clear();

	vector<string> files = ListFilesAtDirectory(".\\Data\\Common\\Planets\\*.txt");
	
	int subPos = files[0].find("Planets\\") + 8;

	for (int i = 0; i < files.size(); i++) {
		string name = files[i].substr(subPos);
		name = name.substr(0, name.size()-5);

		types[name] = PlanetType(("./Data/Common/Planets/" + name + ".txt").c_str());
	}
}


PlanetType* PlanetTypeManager::GetType(std::string name) {
	return &types[name];
}