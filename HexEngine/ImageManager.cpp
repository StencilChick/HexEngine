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

		images[name] = Image(("./Data/Images/" + name).c_str());
	}
}

void ImageManager::Unload() {
	for (std::map<std::string, Image>::iterator it = images.begin(); it != images.end(); it++) {
		it->second.Delete();
	}
}

// get stuff
Image* ImageManager::GetImage(string name) {
	return &images[name];
}
Image* ImageManager::GetImage(const char* name) { return GetImage(string(name)); }