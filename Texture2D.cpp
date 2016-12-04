#include "Main.h"

Texture2D::Texture2D() {
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
	if ((x < 0 || x >= width) ||
		(y < 0 || y >= height))
		return vec4(0, 0, 0, 0);

	return vec4(
		ColorMap[y][x][0],
		ColorMap[y][x][1],
		ColorMap[y][x][2],
		ColorMap[y][x][3]);
}

