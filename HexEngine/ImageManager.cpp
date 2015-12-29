#include "ImageManager.h"

#include <SOIL\SOIL.h>
#include <iostream>

#include "FileLoaders.h"

using namespace std;

ImageManager::ImageManager() {

}

// load/unload stuff
void ImageManager::Load() {
	images.clear();

	vector<string> files = ListFilesAtDirectory(".\\Data\\Images\\*.png");
	
	int subPos = files[0].find("Images\\") + 7;

	for (int i = 0; i < files.size(); i++) {
		string name = files[i].substr(subPos);
		name = name.substr(0, name.size()-1);

		// load it
		GLuint image;
		int width, height;
		glGenTextures(1, &image);
		glBindTexture(GL_TEXTURE_2D, image);

		unsigned char* imageData = SOIL_load_image(("./Data/Images/" + name).c_str(), &width, &height, 0, SOIL_LOAD_RGBA);
		if (imageData == NULL) { cout << "Error loading image " << name << endl; }

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, imageData);
		SOIL_free_image_data(imageData);

		images[name] = image;
	}
}

void ImageManager::Unload() {
	for (map<string, GLuint>::iterator it = images.begin(); it != images.end(); it++) {
		glDeleteTextures(1, &it->second);
	}
}

// get stuff
GLuint ImageManager::GetImage(string name) {
	return images[name];
}
GLuint ImageManager::GetImage(const char* name) { return GetImage(string(name)); }