#include "Planet.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "Star.h"

#include "World.h"
#include "Game.h"
#include "Camera.h"

#include "simplex/simplexnoise.h"
#include "ShaderHelpers.h"

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

	// pseudorandom stuff
	glm::vec3 noiseCentre;
	if (parent == nullptr) {
		noiseCentre = (star->GetPosition() + glm::vec3(distance, 0, 0)) * 100.0f;
	} else {
		noiseCentre = (star->GetPosition() + glm::vec3(parent->GetDistance(), distance*1.0f/4, 0))*100.0f;
	}
	
	// build a terrestial or gas planet
	if (parent == nullptr && distance >= star->GetFrostLine()) {
		BuildGasPlanet(noiseCentre);
	} else {
		BuildTerrestianPlanet(noiseCentre);
	}

	radius = pow(2, size-1);


	// revolution
	startAngle = 2*M_PI * (octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+3)+1)/2;

	if (parent == nullptr) {
		revolveTime = 100*distance + 100*distance/2 * raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+4);
	} else {
		revolveTime = 30*distance + 30*distance/2 * raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+4);
	}

	// moons
	int numMoons = 0;
	if (parent == nullptr) {
		if (isGasPlanet) {
			numMoons = 1 + 3 * (octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+5)+1)/2;
		}
		else if (size == 4 && (raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+5) >= 0.5f || type->needsMoon)) {
			numMoons = 1;
		}
	}

	for (int i = 1; i <= numMoons; i++) {
		Planet *moon = new Planet(star);
		moon->Init(this, i);

		moons.push_back(moon);
	}


	// rotation time
	if (isGasPlanet) {
		rotateTime = 60;
	} else {
		if (type->tidalLock && distance == 1 && moons.size() == 0) {
			rotateTime = revolveTime;
		} else {
			rotateTime = 10 * (1.0f + octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+6)/2);
		}
	}
}

void Planet::BuildTerrestianPlanet(glm::vec3 noiseCentre) {
	isGasPlanet = false;

	float seed = Game::GetGalaxy()->GetSeed();

	// size
	if (raw_noise_4d(noiseCentre.x, noiseCentre.y, noiseCentre.z, seed) >= 0) {
		size = 4;
	} else {
		size = 3;
	}
	if (parent != nullptr) size--;
	

	// assign type
	srand((int)(noiseCentre.x + noiseCentre.y + noiseCentre.z + seed));
	float typeWeight = rand() % Game::GetPlanetTypeManager()->GetWeightVal();
	//float typeWeight = Game::GetPlanetTypeManager()->GetWeightVal() * (octave_noise_4d(1, 0, 10.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+1)+1)/2;

	std::vector<std::string> typeNames = Game::GetPlanetTypeManager()->GetTypeNames();

	int id = 0;
	while (true) {
		PlanetType *t = Game::GetPlanetTypeManager()->GetType(typeNames[id]);

		if (t->IsPlanetValid(this)) {
			if (t->weight > typeWeight) {
				type = t;
				break;
			} else {
				typeWeight -= t->weight;
			}
		}

		id++;
		if (id >= typeNames.size()) { id = 0; }
	}

	// other values
	seaLevel = type->minSeaLevel + (type->maxSeaLevel - type->minSeaLevel) * (octave_noise_4d(4, 0.15f, 1.0f, noiseCentre.x, noiseCentre.y, noiseCentre.z, seed+2)+3)/2;
}

void Planet::BuildGasPlanet(glm::vec3 noiseCentre) {
	isGasPlanet = true;

	size = 6;
}


void Planet::SetUpMesh() {
	if (!meshCreated) {
		if (!isGasPlanet) {
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
		}


		for (std::vector<Planet*>::iterator it = moons.begin(); it != moons.end(); it++) {
			(*it)->SetUpMesh();
		}


		meshCreated = true;
	}
}

void Planet::DeleteMesh() {
	if (meshCreated) {
		if (!isGasPlanet) {
			for (int i = 0; i < 10; i++) {
				delete meshes[i];
				meshes[i] = nullptr;
			}
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

	if (!isGasPlanet) {
		for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
			(*it)->Draw(
				World::GetShaderManager()->GetShader("default"), 
				glm::translate(position) * glm::scale(glm::vec3(radius, radius, radius)) * glm::rotate(GetRotation(), glm::vec3(0, 1, 0)), 
				glm::vec4(1, 1, 1, 1), 
				World::GetImageManager()->GetImage(type->image)
				);
			(*it)->BindBuffersAndDraw();
		}
	} else {
		World::GetMeshManager()->GetMesh("sphere.obj")->Draw(
			World::GetShaderManager()->GetShader("default"),
			glm::translate(position) * glm::scale(glm::vec3(radius, radius, radius)) * glm::rotate(GetRotation(), glm::vec3(0, 1, 0)),
			glm::vec4(1, 1, 1, 1),
			World::GetImageManager()->GetImage("Planets\\gasTest.png")
			);
	}
}


int Planet::GetSizeVal() {
	return size;
}

bool Planet::IsMoon() {
	return parent != nullptr;
}

bool Planet::IsGasPlanet() {
	return isGasPlanet;
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
	float dist  = GetDistanceToOrbitCentre();

	glm::vec3 pos = glm::vec3(
		cos(angle) * dist, 
		0, 
		sin(angle) * dist
		);
	if (parent != nullptr) pos += parent->GetPosition();

	return pos;
}

float Planet::GetRotation() {
	return 2*M_PI / rotateTime * Game::GetGalaxy()->GetTimer()->GetTime();
}

PlanetType* Planet::GetType() {
	return type;
}


// stuff
int Planet::GetHexHeight(glm::vec3 pos) {
	float seed = Game::GetGalaxy()->GetSeed();
	float scale = 1 + 0.1 * size;
	glm::vec3 noiseCentre = (star->GetPosition() + glm::vec3(distance, 0, 0)) * 100.0f;

	int height = std::max(round((
		octave_noise_4d(
			4, 0.15f, 1.0f, 
			pos.x * scale + noiseCentre.x, pos.y * scale + noiseCentre.y, pos.z * scale + noiseCentre.z, seed)
		- seaLevel) * 6), 0.0f);
		height = std::min(5, height);

	int temp = GetHexTemp(pos);
	
	// verify valid height for hex
	while (type->hexes[temp][height] == std::string("no")) {
		height++;
		if (height > 5) height = 0;
	}

	return height;
}

int Planet::GetHexTemp(glm::vec3 pos) {
	float seed = Game::GetGalaxy()->GetSeed();
	float scale = 1 + 0.1 * size;
	glm::vec3 noiseCentre = (star->GetPosition() + glm::vec3(distance, 0, 0)) * 100.0f;

	int temp = std::max(round((1 - abs(pos.y)) * 3 + 
		octave_noise_4d(
			4, 0.15f, 1.0f, 
			pos.x * scale + noiseCentre.x, pos.y * scale + noiseCentre.y, pos.z * scale + noiseCentre.z, seed+1)
		/2) + type->tempMod, 0.0f);
	temp = std::min(2, temp);

	return temp;
}

bool Planet::GetRayHit(glm::vec3 point, glm::vec3 dir, glm::vec3 &result) {
	glm::vec3 pos = GetPosition();
	float rad = radius + 0.075f / sqrt(radius) * 2;

	float a = glm::dot(dir, dir);
	float b = glm::dot(dir, 2.0f*(point-pos));
	float c = glm::dot(pos, pos) + glm::dot(point, point) - 2*glm::dot(pos, point) - rad*rad;

	float d = b*b - 4*a*c;
	if (d < 0) return false;

	result = point + ((-b - sqrt(d)) / 2 * a) * dir;
	return true;
}
bool Planet::GetRayHit(Ray ray, glm::vec3 &result) { return GetRayHit(ray.point, ray.direction, result); }

bool Planet::GetRayHit(glm::vec3 point, glm::vec3 dir) {
	glm::vec3 result;
	return GetRayHit(point, dir, result);
}
bool Planet::GetRayHit(Ray ray) { return GetRayHit(ray.point, ray.direction); }


PlanetHex* Planet::GetClosestHexToPos(glm::vec3 pos) {
	PlanetHex *hex = nullptr;

	std::vector<PlanetHex> hexes = Game::GetSphereManager()->GetHexSphere(size);

	float maxDist = 10000;
	for (int i = 0; i < hexes.size(); i++) {
		float delta = glm::length(hexes[i].pos - pos);
		if (delta < maxDist) {
			maxDist = delta;
			hex = (PlanetHex*)&Game::GetSphereManager()->GetHexSphere(size)[i];
		}
	}

	hex->Assign(this, GetHexHeight(hex->pos), GetHexTemp(hex->pos));
	return hex;
}


float Planet::GetDistanceToOrbitCentre() {
	float dist = 128 / 3 * size;
	if (parent != nullptr) {
		dist /= 3;
		if (!parent->IsGasPlanet()) {
			dist = 0;
		}
	}

	if (distance == 1) {
		int startVal;
		if (parent != nullptr) {
			if (parent->IsGasPlanet()) {
				startVal = 32;
			} else {
				startVal = 32;
			}
		} else {
			startVal = 32;
		}

		dist += startVal;
	} else {
		if (parent == nullptr) {
			dist += star->GetPlanet(distance-2)->GetDistanceToOrbitCentre();
		} else {
			dist += parent->GetMoon(distance-2)->GetDistanceToOrbitCentre();
		}
	}
	
	return dist;
}

void Planet::DrawOrbit(float a) {
	a = std::max(std::min(1.0f, a), 0.0f);
	if (a > 0) {
		ShaderManager *sm = World::GetShaderManager();

		float rad = GetDistanceToOrbitCentre();

		glm::vec3 centre = glm::vec3(0, 0, 0);
		if (parent != nullptr) centre = parent->GetPosition();

		SetShaderM4(sm->GetShader("orbit"), "modelMatrix", mat4());
		SetShaderV4(sm->GetShader("orbit"), "colour", vec4(0.75f, 0.75f, 0.75f, a));

		glLineWidth(1);

		glBegin(GL_LINE_LOOP);
		for (float k = 0; k < M_PI*2; k += M_PI/180) {
			glm::vec3 p = glm::vec3(centre.x + cos(k) * rad, 0, centre.z + sin(k) * rad);

			glVertex3f(p.x, 0, p.z);
		}
		glEnd();
	}
}