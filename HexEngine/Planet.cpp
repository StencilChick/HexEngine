#include "Planet.h"

#include <glm/gtx/transform.hpp>
#include <iostream>

#define _USE_MATH_DEFINES
#include <math.h>
#include <algorithm>

#include "World.h"
#include "simplex/simplexnoise.h"

Planet::Planet() {
	meshes.resize(10);

	pos = glm::vec3(0, 0, 0);
}

Planet::~Planet() {
	for (int i = 0; i < meshes.size(); i++) {
		delete meshes[i];
	}
}


void Planet::SetUp(int subs) {
	size = subs;
	radius = 2 * pow(2, subs-1);

	// vertices
	float t = (1.0f + sqrt(5.0f)) / 2.0f;

	glm::vec3 verts[] = {
		glm::vec3(0, -1, 0),
		glm::vec3(0.723600f, -0.447215f, 0.525720f),
		glm::vec3(-0.276385f, -0.447215f, 0.850640f),
		glm::vec3(-0.894425f, -0.447215f, 0),
		glm::vec3(-0.276385f, -0.447215f, -0.850640f),
		glm::vec3(0.723600f, -0.447215f, -0.525720f),
		glm::vec3(0.276385f, 0.447215f, 0.850640f),
		glm::vec3(-0.723600f, 0.447215f, 0.525720f),
		glm::vec3(-0.723600f, 0.447215f, -0.525720f),
		glm::vec3(0.276385f, 0.447215f, -0.850640f),
		glm::vec3(0.894425f, 0.447215f, 0),
		glm::vec3(0, 1, 0)
	};

	// faces
	tris.resize(20);
	tris[0] = PlanetTri(verts[0], verts[1], verts[2]);
	tris[1] = PlanetTri(verts[1], verts[0], verts[5]);
	tris[2] = PlanetTri(verts[0], verts[2], verts[3]);
	tris[3] = PlanetTri(verts[0], verts[3], verts[4]);
	tris[4] = PlanetTri(verts[0], verts[4], verts[5]);

	tris[5] = PlanetTri(verts[1], verts[5], verts[10]);
	tris[6] = PlanetTri(verts[2], verts[1], verts[6]);
	tris[7] = PlanetTri(verts[3], verts[2], verts[7]);
	tris[8] = PlanetTri(verts[4], verts[3], verts[8]);
	tris[9] = PlanetTri(verts[5], verts[4], verts[9]);

	tris[10] = PlanetTri(verts[1], verts[10], verts[6]);
	tris[11] = PlanetTri(verts[2], verts[6], verts[7]);
	tris[12] = PlanetTri(verts[3], verts[7], verts[8]);
	tris[13] = PlanetTri(verts[4], verts[8], verts[9]);
	tris[14] = PlanetTri(verts[5], verts[9], verts[10]);

	tris[15] = PlanetTri(verts[6], verts[10], verts[11]);
	tris[16] = PlanetTri(verts[7], verts[6], verts[11]);
	tris[17] = PlanetTri(verts[8], verts[7], verts[11]);
	tris[18] = PlanetTri(verts[9], verts[8], verts[11]);
	tris[19] = PlanetTri(verts[10], verts[9], verts[11]);

	// subdivisons
	for (int s = 0; s < subs; s++) {
		std::vector<PlanetTri> newTris;
		newTris.resize(tris.size() * 4);

		for (int i = 0; i < tris.size(); i++) {
			PlanetTri subTris[4] = { };
			tris[i].Subdivide(subTris);

			newTris[i*4] = subTris[0];
			newTris[i*4+1] = subTris[1];
			newTris[i*4+2] = subTris[2];
			newTris[i*4+3] = subTris[3];
		}

		tris = newTris;
	}


	// make hexes from the triangles
	std::vector<glm::vec3> addedVerts;

	int imgWid = World::GetImageManager()->GetImage("planetColours.png")->GetWidth();
	int imgHei = World::GetImageManager()->GetImage("planetColours.png")->GetHeight();

	int seed = rand() % 10000; //23764; //2154;
	float seaLevel = octave_noise_4d(4, 0.15f, 1.0f, 0, 0, 0, seed) / 2.5f;
	std::cout << seaLevel << std::endl;

	for (std::vector<PlanetTri>::iterator it = tris.begin(); it != tris.end(); it++) {
		// assign tris to hexes
		for (int i = 0; i < 3; i++) {
			if (std::find(addedVerts.begin(), addedVerts.end(), it->points[i]) == addedVerts.end()) {
				// not already a hex
				glm::vec3 pos = it->points[i];

				float scale = 1 + 0.1 * size;
				int height = std::max(round((
					octave_noise_4d(
						4, 0.15f, 1.0f, 
						pos.x * scale, pos.y * scale, pos.z * scale, seed)
					- seaLevel) * imgWid), 0.0f);
				int temp = std::max(round((1 - abs(pos.y)) * imgHei + 
					octave_noise_4d(
						4, 0.15f, 1.0f, 
						pos.x * scale, pos.y * scale, pos.z * scale, seed+1)/2),
					0.0f);

				PlanetHex hex = PlanetHex(this, pos, height, temp);
				hex.tris.push_back(it);

				hexes.push_back(hex);

				addedVerts.push_back(it->points[i]);
			} else {
				// already a hex
				for (std::vector<PlanetHex>::iterator hexIt = hexes.begin(); hexIt != hexes.end(); hexIt++) {
					if (hexIt->pos == it->points[i]) {
						hexIt->tris.push_back(it);
						break;
					}
				}
			}
		}
	}

	// mesh
	std::vector<GLfloat> vertices[10];
	std::vector<GLushort> elements[10];

	for (std::vector<PlanetHex>::iterator it = hexes.begin(); it != hexes.end(); it++) {
		int listInd = (atan2(it->pos.z, it->pos.x) / M_PI + 1) / 2 * 5;
		if (listInd == 5) listInd = 0;
		if (it->pos.y < 0) listInd += 5;

		it->AddHexToMesh(vertices[listInd], elements[listInd]);
		//it->AddHexSurfaceToMesh(vertices[listInd], elements[listInd]);
	}

	for (int i = 0; i < 10; i++) {
		meshes[i] = new Mesh(&vertices[i][0], vertices[i].size()/5, &elements[i][0], elements[i].size());
		meshes[i]->AddShader(World::GetShaderManager()->GetShader("default"));
	}
}


void Planet::Update() {

}

void Planet::Draw() {
	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
		(*it)->Draw(
			World::GetShaderManager()->GetShader("default"), 
			glm::translate(pos) * glm::scale(glm::vec3(radius, radius, radius)), 
			glm::vec4(1, 1, 1, 1), 
			World::GetImageManager()->GetImage("planetColours.png")
			);
		(*it)->BindBuffersAndDraw();
	}
}


void Planet::SetPos(glm::vec3 pos) {
	this->pos = pos;
}

glm::vec3 Planet::GetPos() {
	return pos;
}

float Planet::GetRadius() {
	return radius;
}


// stuff
bool Planet::GetRayHit(glm::vec3 point, glm::vec3 dir, glm::vec3 &result) {
	float a = glm::dot(dir, dir);
	float b = glm::dot(2.0f*dir, point);
	float c = glm::dot(point, point) - radius*radius;

	float d = b*b - 4*a*c;
	if (d < 0) return false;

	result = point + ((-b - sqrt(d)) / 2 * a) * dir;
	return true;
}
bool Planet::GetRayHit(Ray ray, glm::vec3 &result) { return GetRayHit(ray.point, ray.direction, result); }


PlanetHex* Planet::GetClosestHexToPos(glm::vec3 pos) {
	PlanetHex *hex = nullptr;

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