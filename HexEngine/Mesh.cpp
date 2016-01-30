#include "Mesh.h"

#include <vector>
#include <iostream>
#include <string>

#include "FileLoaders.h"
#include "ShaderHelpers.h"

#include "World.h"

// constructors
Mesh::Mesh() {
	// [put some default mesh here (probably a plane)]
}

Mesh::Mesh(GLfloat *verts, int vertNum, GLushort *elems, int elemNum) {
	BindGL(verts, vertNum, elems, elemNum);
}

Mesh::Mesh(const char *filename) {
	std::vector<GLfloat> vertices;
	std::vector<GLushort> elements;
	LoadOBJ(filename, vertices, elements);

	BindGL(&vertices[0], vertices.size()/5, &elements[0], elements.size());
}


// destructor
Mesh::~Mesh() {
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &buffer);
	glDeleteBuffers(1, &elements);
}


// sets up GL stuff based on input
void Mesh::BindGL(GLfloat* vertices, int vertexNum, GLushort* elems, int elemNum) {
	glGenVertexArrays(1, &vao);
	glBindVertexArray(vao);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBufferData(
		GL_ARRAY_BUFFER,
		sizeof(float) * 5 * vertexNum,
		vertices,
		GL_STATIC_DRAW
		);

	// position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, 0);
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GL_FLOAT) * 5, (void*)(sizeof(GL_FLOAT) * 3));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &elements);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*elemNum, elems, GL_STATIC_DRAW);
}

// draw
void Mesh::Draw(GLuint shader, const glm::mat4 &modelMatrix, const glm::vec4 &colour, Image* img) {
	// find the id of the value of this image in the array of images this mesh will use
	int imgID;
	vector<Image*>::iterator imgIt = find(images.begin(), images.end(), img);
	if (imgIt == images.end()) {
		images.push_back(img);
		if (img->GetWidth() > imgWidth) imgWidth = img->GetWidth();
		if (img->GetHeight() > imgHeight) imgHeight = img->GetHeight();

		imgID = images.size();
	} else {
		imgID = imgIt - images.begin();
	}

	instanceData[shader].push_back(InstanceData(modelMatrix, colour, imgID));
}

void Mesh::BindBuffersAndDraw() {
	if (images.size() == 0) return;

	// make texture array
	GLuint texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D_ARRAY, texture);

	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, GL_RGBA, imgWidth, imgHeight, images.size(), 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);

	for (int i = 0; i < images.size(); i++) {
		Image *img = images[i];

		glTexSubImage3D(
			GL_TEXTURE_2D_ARRAY, 
			0, 
			0, 0, i, 
			img->GetWidth(), img->GetHeight(), 1, 
			GL_RGBA, 
			GL_UNSIGNED_BYTE, 
			img->GetImageData()
			);
	}

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// bind the buffers and junk
	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elements);

	int size; glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &size);

	ShaderManager *shaderMgr = World::GetShaderManager();

	// Zhu Li, do the thing!
	map<GLuint, vector<InstanceData>>::iterator it;
	for (it = instanceData.begin(); it != instanceData.end(); it++) {
		if (it->second.size() == 0) continue;

		glUseProgram(it->first);

		for (int i = 0; i < it->second.size(); i++) {
			int index = i;
			while (index >= shaderMgr->INSTANCE_LENGTH) index -= shaderMgr->INSTANCE_LENGTH;

			InstanceData data = it->second[i];
			InstanceLoc loc = shaderMgr->GetInstanceLoc(it->first, index);

			SetShaderM4(it->first, loc.modelLoc, data.modelMatrix);
			SetShaderV4(it->first, loc.colourLoc, data.colour);
			SetShaderInt(it->first, loc.texIdLoc, data.imageID);

			if ((i+1) % shaderMgr->INSTANCE_LENGTH == 0) {
				glDrawElementsInstanced(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0, shaderMgr->INSTANCE_LENGTH);
			}
		}

		int length = it->second.size();
		while (length >= shaderMgr->INSTANCE_LENGTH) length -= shaderMgr->INSTANCE_LENGTH;
		glDrawElementsInstanced(GL_TRIANGLES, size/sizeof(GLushort), GL_UNSIGNED_SHORT, 0, length);

		it->second.clear();
	}

	// clean up
	glDeleteTextures(1, &texture);

	images.clear();
	imgWidth = 0;
	imgHeight = 0;
}


void Mesh::AddShader(GLuint shader) {
	vector<InstanceData> v;
	instanceData[shader] = v;
}