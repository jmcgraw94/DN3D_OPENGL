#include "Main.h"
#include "Shader.h"
#include "Camera.h"
#include "Blurb.h"
#include "ContentManager.h"

#include "..\include\SOIL.h"
#include "..\include\GL\glew.h"
#include "..\include\math\matrix4d.h"
#include "..\include\math\vect3d.h"
#include "..\include\glfw3.h"
#include "..\include\glm\glm.hpp"
#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"
#include "..\include\MapFactory.h"

#include <stdlib.h>
#include <time.h>
#include <GL\glut.h>
#include <GL\GL.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>


using namespace std;
using namespace glm;

//Extern
int W_WIDTH = 800;
int W_HEIGHT = 600;

//Static
bool Main::keys[1024];
GLFWwindow * Main::window;
Camera Main::MainCamera;
ContentManager Main::CM = ContentManager();
MapFactory Main::MF = MapFactory();


//Private
vec2 MousePos, OldMousePos;
vector<Blurb> Blurbs = vector<Blurb>();

// Is called whenever a key is pressed/released via GLFW
void Main::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		Main::keys[key] = true;
	else if (action == GLFW_RELEASE)
		Main::keys[key] = false;


	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void Main::mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	MousePos.x = xpos;
	MousePos.y = ypos;

	//cout << MousePos.x << "," << MousePos.y << endl;
}

void Main::Setup() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	Main::window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "RogueGL", nullptr, nullptr);
	glfwMakeContextCurrent(Main::window);

	// Set the required callback functions
	glfwSetKeyCallback(Main::window, Main::key_callback);
	glfwSetCursorPosCallback(Main::window, Main::mouse_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int _w, _h;
	glfwGetFramebufferSize(Main::window, &_w, &_h);
	glViewport(0, 0, _w, _h);

	MainCamera = Camera();

	for (int i = 0; i < 5; i++) {
		Blurb B = Blurb(vec3(i, 0, 0), i % 3 + 1);
		B.Init();
		Blurbs.push_back(B);
	}

	//for (int x = 0; x < MF.w; x++) {
	//	for (int y = 0; y < MF.h; y++) {
	//		if (MF.GetPixelAt(x, y) == vec3(0, 0, 0)) {
	//			Blurb B = Blurb(vec3(x, 0, y), 1);
	//			Blurbs.push_back(B);
	//		}
	//		if (MF.GetPixelAt(x, y) == vec3(255, 0, 0)) {
	//			Blurb B = Blurb(vec3(x, 0, y), 3);
	//			Blurbs.push_back(B);
	//		}
	//		if (MF.GetPixelAt(x, y) == vec3(255, 255, 255)) {
	//			{
	//				Blurb B = Blurb(vec3(x, -1, y), 1);
	//				Blurbs.push_back(B);
	//			}
	//			{
	//				Blurb B = Blurb(vec3(x, 1, y), 2);
	//				Blurbs.push_back(B);
	//			}
	//		}
	//	}
	//}

	cout << "SETUP COMPLETE\n" << endl;
}

void Main::Update() {
	glfwPollEvents();
	Main::MainCamera.Update();

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i].Update();
	}
}

void Main::Draw() {
	glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i].Draw();
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Main::LateUpdate() {
	glfwSwapBuffers(Main::window);
	OldMousePos = MousePos;
}

int main()
{
	Main::Setup();

	while (!glfwWindowShouldClose(Main::window))
	{
		Main::Update();

		Main::Draw();

		Main::LateUpdate();
	}

	glfwTerminate();

	return 0;
}





