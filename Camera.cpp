#include "Camera.h"
#include "Main.h"
#include "Helper.h"

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

	//Position = glm::vec3(3, 6, 8);
	Position = glm::vec3(0, 0, 0);
	Rotation = glm::vec3(0, 0, 0);

	ForwardVec = glm::vec3(0, 0, -1);
	UpVec = glm::vec3(0, 1, 0);


	cout << "Camera Created" << endl;
}

Camera::~Camera()
{
}

void Camera::Update() {
	view = glm::mat4();
	projection = glm::mat4();

	if (Rotation.x > glm::radians(89.f))
		Rotation.x = glm::radians(89.f);

	Rotation.x = glm::radians(45.0f);

	{
		float rotSpeed = .025;
		GLfloat speed = 0.08f;

		if (Main::PressKeys[GLFW_KEY_W]) {
			//cameraPos += vec3(dx, 0, dy) * speed;
			Position += speed * ForwardVec;
		}

		if (Main::PressKeys[GLFW_KEY_S]) {
			//cameraPos -= vec3(dx, 0, dy) * speed;
			Position -= speed * ForwardVec;
		}

		if (Main::PressKeys[GLFW_KEY_A]) {
			//rot -= rotSpeed;
			//Rotation.y -= 4.0f;
			Position -= glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		}

		if (Main::PressKeys[GLFW_KEY_D]) {
			//rot += rotSpeed;
			//Rotation.y += 4.0f;
			Position += glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		}

		if (Main::PressKeys[GLFW_KEY_RIGHT]) {
			//rot -= rotSpeed;
			Rotation.y += 4.0f;
			//Position += glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		}

		if (Main::PressKeys[GLFW_KEY_LEFT]) {
			//rot += rotSpeed;
			Rotation.y -= 4.0f;
			//Position -= glm::normalize(glm::cross(ForwardVec, UpVec)) * speed;
		}
	}

	//ForwardVec = Rotation;

	//Rotation.y += Main::DeltaMousePos.x / 2;
	//Rotation.x += Main::DeltaMousePos.y / 2;

	//ForwardVec.y = atan2(Rotation.z, Rotation.x);

	//Helper::PrintVec3(Rotation);

	//	Helper::PrintVec3(cameraPos, "Camera Pos");
	//cout << cameraPos.z << endl;
	//view = glm::rotate(view, glm::radians(Rotation.z), vec3(0, 0, 1));

	//view = glm::rotate(view, glm::radians(45.0f), vec3(1, 0, 0));
	view = glm::rotate(view, glm::radians(Rotation.y), vec3(0, 1, 0));


	view = glm::translate(view, -Position);
	projection = glm::perspective(45.0f, (float)WIN_W / (float)WIN_H, 0.1f, 100.0f);
	//projection *= glm::lookAt(vec3(0,0,0), cameraPos, cameraUp);
}
