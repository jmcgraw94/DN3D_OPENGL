#include "Texture2D.h"

Texture2D::Texture2D(){
}

Texture2D::Texture2D(string path)
{
	Pixels = Main::CM.LoadPixels(path, &width, &height);
	ColorMap = Main::CM.CreatePixelGrid(path, &width, &height);


}

Texture2D::~Texture2D()
{
}

vec4 Texture2D::GetPixel(int x, int y) {
	return vec4(
		ColorMap[x][y][0],
		ColorMap[x][y][1],
		ColorMap[x][y][2],
		ColorMap[x][y][3]);
}
