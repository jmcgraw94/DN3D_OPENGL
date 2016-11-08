#include <MapFactory.h>


MapFactory::MapFactory() {}
MapFactory::~MapFactory() {}

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

	int r = 0;
	int c = 0;
	int x = 0;

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


vec3 MapFactory::GetPixelAt(int x, int y) {
	return vec3(
		ColorArray[x][y][0],
		ColorArray[x][y][1],
		ColorArray[x][y][2]);
}

void MapFactory::LoadMap()
{
	const string fileName = "Content/Test.png";

	vector<unsigned char> decodeResult;

	unsigned RESULT = decode(decodeResult, w, h, fileName);

	cout << (RESULT == 0 ? "Success!" : "Failure") << endl;

	int x = 2;
	int y = 0;

	ColorArray = CreateVectorGrid(decodeResult, w, h);

	cout << ColorArray[x][y][0] << "," <<
		ColorArray[x][y][1] << "," <<
		ColorArray[x][y][2] << endl;
}






