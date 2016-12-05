#pragma once

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
};

