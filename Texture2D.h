#pragma once
#include "SOIL.h"
#include "lodepng.h"
#include "Main.h"

#include "..\include\glm\glm.hpp"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>


using namespace std;
using namespace glm;

class Texture2D
{
public:
	Texture2D();

	Texture2D(string path);

	vec4 GetPixel(int x, int y);

	unsigned int
		width,
		height;

	float *
		Pixels;
	float ***
		ColorMap;

	~Texture2D();

private:
	vector<vector<vector<int>>>
		ColorArray;
};

