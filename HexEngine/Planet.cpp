#include "Planet.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "Star.h"

#include "World.h"
#include "Game.h"
#include "simplex/simplexnoise.h"

Planet::Planet() {
	meshes.resize(10);
	meshCreated = false;
}
Planet::Planet(Star *star) : Planet() {
	this->star = star;
}

Planet::~Planet() {
	for (std::vector<Planet*>::iterator it = moons.begin(); it != moons.end(); it++) {
		delete *it;
	}
	moons.clear();
}


void Planet::Init(Planet *parent, int distance) {
	this->parent = parent;
	this->distance = distance;
	int seed = Game::GetGalaxy()->GetSeed();

	// assign type pseudorandomly
	glm::vec3 noiseCentre;
	if (parent == nullptr) {
		noiseCentre = (star->GetPosition() + glm::vec3(distance, 0, 0)) * 100.0f;
	} else {
		noiseCentre = (star->GetPosition() + glm::vec3(parent->GetDistance(), distance*1.0f/4, 0))*100.0f;
	}
	
	if (raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed) >= 0) {
		size = 4;
	} else {
		size = 3;
	}
	if (parent != nullptr) size--;
	radius = pow(2, size-1);

	float typeWeight = Game::GetPlanetTypeManager()->GetWeightVal() * (octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+1)+1)/2;
	if (typeWeight >= Game::GetPlanetTypeManager()->GetWeightVal()) typeWeight -= 0.1f;

	std::vector<std::string> typeNames = Game::GetPlanetTypeManager()->GetTypeNames();
	for (std::vector<std::string>::iterator it = typeNames.begin(); it != typeNames.end(); it++) {
		PlanetType *t = Game::GetPlanetTypeManager()->GetType(*it);

		if (t->weight > typeWeight) {
			type = t;
			break;
		} else {
			typeWeight -= t->weight;
		}
	}

	// assign other vals pseudorandomly
	startAngle = 2*M_PI * (octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+2)+1)/2;
	seaLevel = type->minSeaLevel + (type->maxSeaLevel - type->minSeaLevel) * (octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+2)+3)/2;

	if (parent == nullptr) {
		revolveTime = 100*distance + 100*distance/2 * raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+4);
	} else {
		revolveTime = 30*distance + 30*distance/2 * raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+4);
	}

	// add a moon, maybe
	if (parent == nullptr && size == 4) {
		//if (raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+5) > 0.5f) {
			Planet *moon = new Planet(star);
			moon->Init(this, 1);

			moons.push_back(moon);
		//}
	}
}

void Planet::SetUpMesh() {
	if (!meshCreated) {
		std::vector<PlanetHex> hexes = Game::GetSphereManager()->GetHexSphere(size);

		std::vector<GLfloat> vertices[10];
		std::vector<GLushort> elements[10];

		for (std::vector<PlanetHex>::iterator it = hexes.begin(); it != hexes.end(); it++) {
			it->Assign(this, GetHexHeight(it->pos), GetHexTemp(it->pos));

			int listInd = (atan2(it->pos.z, it->pos.x) / M_PI + 1) / 2 * 5;
			if (listInd == 5) listInd = 0;
			if (it->pos.y < 0) listInd += 5;

			it->AddHexToMesh(vertices[listInd], elements[listInd]);
		}

		for (int i = 0; i < 10; i++) {
			meshes[i] = new Mesh(&vertices[i][0], vertices[i].size()/5, &elements[i][0], elements[i].size());
			meshes[i]->AddShader(World::GetShaderManager()->GetShader("default"));
		}


		for (std::vector<Planet*>::iterator it = moons.begin(); it != moons.end(); it++) {
			(*it)->SetUpMesh();
		}


		meshCreated = true;
	}
}

void Planet::DeleteMesh() {
	if (meshCreated) {
		for (int i = 0; i < 10; i++) {
			delete meshes[i];
			meshes[i] = nullptr;
		}


		for (std::vector<Planet*>::iterator it = moons.begin(); it != moons.end(); it++) {
			(*it)->DeleteMesh();
		}


		meshCreated = false;
	}
}


void Planet::Update() {

}

void Planet::Draw() {
	glm::vec3 position = GetPosition();

	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		(*it)->Draw(
			World::GetShaderManager()->GetShader("default"), 
			glm::translate(position) * glm::scale(glm::vec3(radius, radius, radius)) * glm::rotate(GetRotation(), glm::vec3(0, 1, 0)), 
			glm::vec4(1, 1, 1, 1), 
			World::GetImageManager()->GetImage(type->image)
			);
		(*it)->BindBuffersAndDraw();
	}

	for (std::vector<Planet*>::iterator it = moons.begin(); it != moons.end(); it++) {
		(*it)->Draw();
	}
}


int Planet::GetMoonCount() {
	return moons.size();
}

Planet* Planet::GetMoon(int i) {
	return moons[i];
}


float Planet::GetRadius() {
	return radius;
}

int Planet::GetDistance() {
	return distance;
}

glm::vec3 Planet::GetPosition() {
	float angle = startAngle + Game::GetGalaxy()->GetTimer()->GetTime() * -2*M_PI/revolveTime;

	float dist;
	if (parent == nullptr) {
		dist = 115 * distance;
	} else {
		dist = 25 * distance;
	}

	glm::vec3 pos = glm::vec3(
		cos(angle) * dist, 
		0, 
		sin(angle) * dist
		);
	if (parent != nullptr) pos += parent->GetPosition();

	return pos;
}

float Planet::GetRotation() {
	return 2*M_PI / 28 * Game::GetGalaxy()->GetTimer()->GetTime();
}

PlanetType* Planet::GetType() {
	return type;
}


// stuff
float Planet::GetHexHeight(glm::vec3 pos) {
	float seed = Game::GetGalaxy()->GetSeed();
	float scale = 1 + 0.1 * size;
	glm::vec3 noiseCentre = (star->GetPosition() + glm::vec3(distance, 0, 0)) * 100.0f;

	int height = std::max(round((
		octave_noise_4d(
			4, 0.15f, 1.0f, 
			pos.x * scale + noiseCentre.x, pos.y * scale + noiseCentre.y, pos.z * scale + noiseCentre.z, seed)
		- seaLevel) * 6), 0.0f);
	height = std::min(6, height);

	return height;
}

float Planet::GetHexTemp(glm::vec3 pos) {
	float seed = Game::GetGalaxy()->GetSeed();
	float scale = 1 + 0.1 * size;
	glm::vec3 noiseCentre = (star->GetPosition() + glm::vec3(distance, 0, 0)) * 100.0f;

	int temp = std::max(round((1 - abs(pos.y)) * 3 + 
		octave_noise_4d(
			4, 0.15f, 1.0f, 
			pos.x * scale + noiseCentre.x, pos.y * scale + noiseCentre.y, pos.z * scale + noiseCentre.z, seed+1)
		/2) + type->tempMod, 0.0f);
	temp = std::min(3, temp);

	return temp;
}

bool Planet::GetRayHit(glm::vec3 point, glm::vec3 dir, glm::vec3 &result) {
	glm::vec3 pos = GetPosition();

	float a = glm::dot(dir, dir);
	float b = glm::dot(dir, 2.0f*(point-pos));
	float c = glm::dot(pos, pos) + glm::dot(point, point) - 2*glm::dot(pos, point) - radius*radius;

	float d = b*b - 4*a*c;
	if (d < 0) return false;

	result = point + ((-b - sqrt(d)) / 2 * a) * dir;
	return true;
}
bool Planet::GetRayHit(Ray ray, glm::vec3 &result) { return GetRayHit(ray.point, ray.direction, result); }

bool Planet::GetRayHit(glm::vec3 point, glm::vec3 dir) {
	glm::vec3 pos = GetPosition();

	float a = glm::dot(dir, dir);
	float b = glm::dot(dir, 2.0f*(point-pos));
	float c = glm::dot(pos, pos) + glm::dot(point, point) - 2*glm::dot(pos, point) - radius*radius;

	float d = b*b - 4*a*c;
	if (d < 0) return false;

	return true;
}
bool Planet::GetRayHit(Ray ray) { return GetRayHit(ray.point, ray.direction); }


// remember to assign after this
PlanetHex* Planet::GetClosestHexToPos(glm::vec3 pos) {
	PlanetHex *hex = nullptr;

	std::vector<PlanetHex> hexes = Game::GetSphereManager()->GetHexSphere(size);

	float maxDist = 10000;
	for (std::vector<PlanetHex>::iterator it = hexes.begin(); it != hexes.end(); it++) {
		float delta = glm::length(it->pos - pos);
		if (delta < maxDist) {
			maxDist = delta;
			hex = &*it;
		}
	}

	return hex;
}