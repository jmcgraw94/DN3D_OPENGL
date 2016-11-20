#pragma once
#include "Shader.h"
#include "GameObject.h"

#include "..\include\math\vect3d.h"
#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"

#include <string>

class PointLight : GameObject
{
public:
	PointLight();
	PointLight::PointLight(vec3 _pos, vec3 _color, float _range, float _brightness);
	~PointLight();

	void Update();

	vec3 Position;
	float Range;
	float Brightness;
	vec3 Color;
};

