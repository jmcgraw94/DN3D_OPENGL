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

	map<string, unsigned char*> Textures;

	void Init();
	void LoadTexture(string fileName);

	~ContentManager();
};

