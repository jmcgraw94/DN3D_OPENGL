#include "ContentManager.h"

ContentManager::ContentManager()
{
}


ContentManager::~ContentManager()
{
}

unsigned char * ContentManager::LoadTexture(string fileName, int * w, int * h) {
	/*if (ContentManager::Collection_Textures.find(fileName) != ContentManager::Collection_Textures.end()) {
		cout << fileName << ": GETTING FILE FROM COLLECTION " << endl;
		return ContentManager::Collection_Textures[fileName];
	}
	else {*/
		cout << fileName << ": ADDING TO THE COLLECTION" << endl;

		char _textureFileChar[1024];
		strcpy_s(_textureFileChar, fileName.c_str());
		unsigned char * NewTexture = SOIL_load_image(_textureFileChar, w, h, 0, SOIL_LOAD_RGBA);

		ContentManager::Collection_Textures.insert({ fileName, NewTexture });
		//SOIL_free_image_data(NewTexture);

		return NewTexture;
	//}
}