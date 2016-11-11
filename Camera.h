#pragma once
#include "glm\glm.hpp"

class Camera
{
public:
	Camera();

	void Update();
	void Init();

	glm::mat4 view;
	glm::mat4 projection;

	glm::vec3 Position;
	glm::vec3 Rotation;

	glm::vec3 ForwardVec;
	glm::vec3 UpVec;

	~Camera();
};

