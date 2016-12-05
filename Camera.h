#pragma once
#include "glm\glm.hpp"

using namespace glm;

class Camera
{
public:
	Camera();

	void Update();
	void Init();

	mat4 view;
	mat4 projection;

	vec3 Position;
	vec3 Rotation;
	vec3 Offset;


	vec3 ForwardVec;
	vec3 UpVec;

	~Camera();
};

