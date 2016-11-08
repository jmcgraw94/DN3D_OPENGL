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

	glm::vec3 cameraPos;
	glm::vec3 cameraFront;
	glm::vec3 cameraUp;

	float rot;

	~Camera();
};

