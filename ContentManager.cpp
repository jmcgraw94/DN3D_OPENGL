#include "ContentManager.h"
#include "MapFactory.h"


ContentManager::ContentManager()
{
}

ContentManager::~ContentManager()
{
}

float * ContentManager::GetImage(string path, unsigned int * w, unsigned int * h, unsigned int channels) {
	if (Collection_Textures.find(path) != Collection_Textures.end()) {
		cout << path << ": GETTING FILE FROM COLLECTION " << endl;
		return Collection_Textures[path];
	}
	else {
		cout << path << ": ADDING NEW FILE TO COLLECTION " << endl;
		vector<unsigned char> _decodeResult;
		decode(_decodeResult, (*w), (*h), path);

		int size = (*w) * (*h) * channels;
		cout << "Size: " << size << endl;
		float * InputArray = new float[size];

		for (int i = 0; i < size; i++) {
			InputArray[i] = (_decodeResult[i] / 255.0f);
		}

		Collection_Textures.insert({ path, InputArray });

		return InputArray;
	}
}



