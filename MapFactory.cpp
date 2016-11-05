#define _USE_MATH_DEFINES
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <math.h>
#include <time.h>
#include <string>
#include <vector>		
#include <GL/glew.h>
#include <GL/glut.h>
#include <GL/glut.h>
#include <MapFactory.h>
#include "math/vect3d.h"
#include "lodepng.h"

using namespace std;
using namespace lodepng;

vector<vector<vector<int>>> ColorArray;

MapFactory::MapFactory() {}
MapFactory::~MapFactory() {}

vector<vector<vector<int>>> CreateGrid(vector<unsigned char> image, int w, int h) {

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

		//if (i > 0 && i % step == 0) {
			//cout << "[" << r + 1 << "," << c + 1 << "] : " << x << endl;
		//}
		//cout << i << " : " << x << endl;

		Grid[c][r][x] = (int)image[i];
		x++;
	}

	return Grid;
}

vector<int> GetPixelAt(vector<unsigned char> image, int x, int y, int w, int h) {
	vector<int> Pixel = vector<int>();
	return Pixel;
}

void MapFactory::LoadMap()
{
	vector <unsigned char> buffer;
	const string fileName = "Content/Test.png";

	vector<unsigned char> image;
	unsigned int w, h;
	unsigned RESULT = decode(image, w, h, fileName);

	cout << (RESULT == 0 ? "Success!" : "Failure") << endl;

	int x = 2;
	int y = 0;

	ColorArray = CreateGrid(image, w, h);

	cout << ColorArray[x][y][0] << "," <<
		ColorArray[x][y][1] << "," <<
		ColorArray[x][y][2] << endl;
}





