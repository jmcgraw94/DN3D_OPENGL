#include "ContentManager.h"
#include "MapFactory.h"


ContentManager::ContentManager()
{
}

ContentManager::~ContentManager()
{
}

float * ContentManager::LoadPixels(string path, unsigned int * w, unsigned int * h, unsigned int channels) {
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

vector<vector<vector<int>>> CreateVectorGrid(vector<unsigned char> image, int w, int h) {

	vector<vector<vector<int>>> Grid;

	for (int i = 0; i < h; i++) {
		vector<vector<int>> Row;

		for (int k = 0; k < w; k++) {
			vector<int> DefColors;

			DefColors.push_back(0);
			DefColors.push_back(0);
			DefColors.push_back(0);
			DefColors.push_back(0);

			Row.push_back(DefColors);
		}
		Grid.push_back(Row);
	}

	int Area = w * h;
	int ColorComponents = 4;

	int r = 0,
		c = 0,
		x = 0;

	for (int i = 0; i < ColorComponents * Area; i++) {
		if (i > 0 && i % ColorComponents == 0) {
			x = 0;
			c++;
		}
		if (c >= w) {
			c = 0;
			r++;
		}

		Grid[c][r][x] = (int)image[i];
		x++;
	}

	return Grid;
}




