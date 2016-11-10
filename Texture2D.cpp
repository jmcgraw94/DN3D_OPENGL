#include "Texture2D.h"

Texture2D::Texture2D(){
}

Texture2D::Texture2D(string path)
{
	Pixels = Main::CM.LoadPixels(path, &width, &height);
}

Texture2D::~Texture2D()
{
}

vec3 Texture2D::GetPixel(int x, int y) {
	return vec3(
		ColorArray[x][y][0],
		ColorArray[x][y][1],
		ColorArray[x][y][2]);
}
