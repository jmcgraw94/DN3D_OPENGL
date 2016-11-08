#pragma once
#include "SOIL.h"
#include "lodepng.h"

#include <stdlib.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <map>
#include <iostream>

using namespace std;

class ContentManager
{
public:
	ContentManager();

	float * GetImage(string path, unsigned int * w, unsigned int * h, unsigned int channels = 4);

	map<string, float *>
		Collection_Textures;


	~ContentManager();
};

