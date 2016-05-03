#pragma once

#include <string>
#include <vector>
#include <map>

class Planet;

struct PlanetType {
	std::string name;
	std::string description;
	int weight;

	std::string image;
	std::string hexes[3][6];

	float minSeaLevel;
	float maxSeaLevel;

	float tempMod;

	bool largeOnly;
	bool smallOnly;

	bool needsMoon;

	PlanetType();
	PlanetType(const char*);

	bool IsPlanetValid(Planet*);

private:
	void ParseLine(const char *line);
};

class PlanetTypeManager {
public:
	PlanetTypeManager();

	void Load();

	PlanetType* GetType(std::string name);
	std::vector<std::string> GetTypeNames();
	int GetWeightVal();

private:
	std::map<std::string, PlanetType> types;

	int weightVal;
};