#pragma once
#include "SOIL.h"
#include "lodepng.h"

#include "..\include\glm\glm.hpp"
#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;
using namespace glm;

class ContentManager
{
public:
	ContentManager();

	float * LoadPixels(string path, 
		unsigned int * w, unsigned int * h, 
		unsigned int channels = 4);

	map<string, float *>
		Textures_Pixels;
	map<string, vec2>
		Textures_Sizes;


	~ContentManager();
};

