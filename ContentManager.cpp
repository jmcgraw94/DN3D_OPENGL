#include "ContentManager.h"



ContentManager::ContentManager()
{
}


ContentManager::~ContentManager()
{
}

void ContentManager::LoadTexture(string fileName) {
	int texWidth, texHeight;
	unsigned char* TextureObject;

	char _textureFileChar[1024];
	strcpy_s(_textureFileChar, fileName.c_str());

	TextureObject = SOIL_load_image(_textureFileChar, &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);

	Textures.insert({ fileName, TextureObject });
}