#include "Planet.h"

#include <glm/gtx/transform.hpp>

#include <iostream>

#include "World.h"

Planet::Planet() {
	mesh = nullptr;

	pos = glm::vec3(0, 0, 0);
}

Planet::~Planet() {
	if (mesh != nullptr) {
		delete mesh;
		mesh = nullptr;
	}
}


void Planet::SetUp(int subs) {
	//size = 0.5f * pow(2, subs-1);
	size = 2 * pow(2, subs-1);

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
	for (std::vector<PlanetTri>::iterator it = tris.begin(); it != tris.end(); it++) {
		for (int i = 0; i < 3; i++) {
			if (std::find(addedVerts.begin(), addedVerts.end(), it->points[i]) == addedVerts.end()) {
				// not already a hex
				PlanetHex hex = PlanetHex(it->points[i]);
				hex.height = (rand() % 5) / 50.0f;
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
	std::vector<GLfloat> vertices;
	std::vector<GLushort> elements;

	for (std::vector<PlanetHex>::iterator it = hexes.begin(); it != hexes.end(); it++) {
		it->AddHexToMesh(vertices, elements);
	}

	if (mesh == nullptr) {
		mesh = new Mesh(&vertices[0], vertices.size()/5, &elements[0], elements.size());
		mesh->AddShader(World::GetShaderManager()->GetShader("default"));
	}
}


void Planet::Update() {

}

void Planet::Draw() {
	glBindTexture(GL_TEXTURE_2D, World::GetImageManager()->GetImage("testGradient.png"));

	mesh->Draw(World::GetShaderManager()->GetShader("default"), glm::translate(pos) * glm::scale(glm::vec3(size, size, size)), glm::vec4(1, 1, 1, 1));
	mesh->BindBuffersAndDraw();
}


void Planet::SetPos(glm::vec3 pos) {
	this->pos = pos;
}