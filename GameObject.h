#pragma once
#include "ShaderPipeline.h"

#include "..\include\math\vect3d.h"
#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"

#include <string>

using namespace std;
using namespace glm;

class GameObject
{
public:
	GameObject();
	~GameObject();

	virtual void Buffer();
	virtual void Init();
	virtual void SetUniforms();
	virtual void UpdateModelMatrix();

	virtual void Update();
	virtual void Draw();

	vec3 Origin = vec3(0, 0, 0);

	bool Constructed = false;
	string Name;
	int ID = 0;

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	bool Dynamic;

	bool isInit;
};

