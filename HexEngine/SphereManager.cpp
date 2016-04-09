#include "SphereManager.h"

SphereManager::SphereManager() {

}

void SphereManager::Init() {
	// == initial icosphere ==
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
	std::vector<PlanetTri> tris;
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

	// == populate hexSpheres with subdivisions ==
	for (int s = 1; s <= 4; s++) {
		// subdivide the triangles
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


		// turn triangles into hexes and store, if appropriate
		if (s >= 2) {
			std::vector<PlanetHex> hexes;
			std::vector<glm::vec3> addedVerts;
			for (std::vector<PlanetTri>::iterator it = tris.begin(); it != tris.end(); it++) {
				// assign tris to hexes
				for (int i = 0; i < 3; i++) {
					if (std::find(addedVerts.begin(), addedVerts.end(), it->points[i]) == addedVerts.end()) {
						// not already a hex
						glm::vec3 pos = it->points[i];

						PlanetHex hex = PlanetHex(pos);
						hex.triPos.push_back(it->centre);

						hexes.push_back(hex);

						addedVerts.push_back(it->points[i]);
					} else {
						// already a hex
						for (std::vector<PlanetHex>::iterator hexIt = hexes.begin(); hexIt != hexes.end(); hexIt++) {
							if (hexIt->pos == it->points[i]) {
								hexIt->triPos.push_back(it->centre);
								break;
							}
						}
					}
				}
			}

			hexSpheres[s] = hexes;
		}
	}
}

const std::vector<PlanetHex>& SphereManager::GetHexSphere(int i) {
	return hexSpheres[i];
}