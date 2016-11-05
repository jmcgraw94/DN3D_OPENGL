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
	cameraPos = glm::vec3(0, 0, -3);
	cameraFront = glm::vec3(0, 0, -1);
	cameraUp = glm::vec3(0, 1, 0);

	cout << "Camera Created" << endl;
}

Camera::~Camera()
{
}

void Camera::Update() {
	view = glm::mat4();
	projection = glm::mat4();


	{
		GLfloat cameraSpeed = 0.05f;
		if (Main::keys[GLFW_KEY_W])
			cameraPos -= cameraSpeed * cameraFront;
		if (Main::keys[GLFW_KEY_S])
			cameraPos += cameraSpeed * cameraFront;
		if (Main::keys[GLFW_KEY_A])
			cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
		if (Main::keys[GLFW_KEY_D])
			cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
	}

	view = glm::translate(view, cameraPos);
	projection = glm::perspective(45.0f, (float)W_WIDTH / (float)W_HEIGHT, 0.1f, 100.0f);

}
