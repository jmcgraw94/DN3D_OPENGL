#include "ContentManager.h"
#include "MapFactory.h"


ContentManager::ContentManager()
{
}

ContentManager::~ContentManager()
{
}

vector<vector<vector<int>>> ContentManager::CreateVectorGrid(vector<unsigned char> image, int w, int h) {

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

float *** ContentManager::CreatePixelGrid(string path, unsigned int * w, unsigned int * h, unsigned int channels) {
	vector<unsigned char> _decodeResult;

	decode(_decodeResult, *w, *h, path);

	int size = (*w) * (*h) * channels;

	float * InputArray = new float[size];

	int i = 0;
	float *** ColorArray = new float **[*w];
	for (int x = 0; x < *w; x++) { //For each column
		ColorArray[x] = new float *[*h]; //The column is a new float array of size h
		for (int y = 0; y < *h; y++) { //for each row
			ColorArray[x][y] = new float [channels];
			for (int v = 0; v < channels; v++) {
				ColorArray[x][y][v] = (_decodeResult[i] / 255.0f);
				i++;
			}

		}
	}

	return ColorArray;
}

float * ContentManager::LoadPixels(string path, unsigned int * w, unsigned int * h, unsigned int channels) {
	if (Textures_Pixels.find(path) != Textures_Pixels.end()) {

		*w = (unsigned int)Textures_Sizes[path].x;
		*h = (unsigned int)Textures_Sizes[path].y;

		return Textures_Pixels[path];
	}
	else {

		vector<unsigned char> _decodeResult;
		decode(_decodeResult, *w, *h, path);

		int size = (*w) * (*h) * channels;

		float * InputArray = new float[size];

		float * ColorGridX = new float[*w];
		float * ColorGridY = new float[*h];


		for (int i = 0; i < size; i++) {
			InputArray[i] = (_decodeResult[i] / 255.0f);
		}

		Textures_Sizes.insert({ path, vec2((unsigned int)* w,(unsigned int)* h) });
		Textures_Pixels.insert({ path, InputArray });

		CreateVectorGrid(_decodeResult, *w, *h);

		cout << "LOADING NEW ASSET: " << path << endl;
		return InputArray;
	}

}






