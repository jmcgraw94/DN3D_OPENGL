#pragma once
#include "SOIL.h"

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
	
	unsigned char * LoadTexture(
		string fileName, int * w, int * h);

	map<string, unsigned char *> 
		Collection_Textures;

	~ContentManager();
};

