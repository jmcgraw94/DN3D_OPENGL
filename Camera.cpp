#include "Camera.h"
#include "Main.h"

#include "..\include\glm\glm.hpp"
#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"

#include <stdlib.h>
#include <time.h>
#include <GL\glut.h>
#include <GL\GL.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;

bool init = false;

Camera::Camera()
{
	view = glm::mat4();
	projection = glm::mat4();

	cameraPos = glm::vec3(0, 0, -3);
	cameraFront = glm::vec3(0, 0, 1);
	cameraUp = glm::vec3(0, 1, 0);

	cout << "Camera Created" << endl;

	rot = 0;
}

Camera::~Camera()
{
}

void Camera::Update() {
	view = glm::mat4();
	projection = glm::mat4();

	float theta = rot;
	float dx = cos(theta);
	float dy = sin(theta);


	{
		float rotSpeed = .025;
		GLfloat speed = 0.05f;

		if (Main::keys[GLFW_KEY_W])
			//cameraPos += vec3(dx, 0, dy) * speed;
			cameraPos += speed * cameraFront;
		if (Main::keys[GLFW_KEY_S])
			//cameraPos -= vec3(dx, 0, dy) * speed;
			cameraPos -= speed * cameraFront;
		if (Main::keys[GLFW_KEY_A])
			//rot -= rotSpeed;
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
		if (Main::keys[GLFW_KEY_D])
			//rot += rotSpeed;
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * speed;
	}

	view = glm::rotate(view, -rot, vec3(0, 1, 0));
	view = glm::translate(view, cameraPos);
	projection = glm::perspective(45.0f, (float)WIN_W / (float)WIN_H, 0.1f, 100.0f);
	//projection *= glm::lookAt(vec3(0,0,0), cameraPos, cameraUp);
}
