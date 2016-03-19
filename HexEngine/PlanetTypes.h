#pragma once

#include <string>
#include <map>

struct PlanetType {
	std::string name;
	std::string description;

	std::string image;
	std::string hexes[3][6];

	float minSeaLevel;
	float maxSeaLevel;

	PlanetType();
	PlanetType(const char*);

private:
	void ParseLine(const char *line);
};

class PlanetTypeManager {
public:
	PlanetTypeManager();

	void Load();

	PlanetType* GetType(std::string name);

private:
	std::map<std::string, PlanetType> types;
};