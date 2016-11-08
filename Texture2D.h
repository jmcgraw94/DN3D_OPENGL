#pragma once
#include "SOIL.h"
#include "lodepng.h"
#include "Main.h"

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;

class Texture2D
{
public:
	Texture2D();

	Texture2D(string path);

	vec3 GetPixelAt(int x, int y);

	unsigned int
		width,
		height;
	
	float * 
		TextureData;
	
	~Texture2D();

private:
	vector<vector<vector<int>>> 
		ColorArray;
};

