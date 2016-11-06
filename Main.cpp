#include "Main.h"
#include "Shader.h"
#include "Camera.h"
#include "Blurb.h"

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

//Extern
int W_WIDTH = 800;
int W_HEIGHT = 600;

//Static
bool Main::keys[1024];
GLFWwindow * Main::window;
Camera Main::MainCamera = Camera();

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

	//cout << key << endl;

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
	Main::window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "LearnOpenGL", nullptr, nullptr);
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


	// Set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// Set texture wrapping to GL_REPEAT (usually basic wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// Set texture filtering parameters
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// Set nearest neighbor filtering
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	// Load image, create texture and generate mipmaps

	Blurb A = Blurb();
	Blurb B = Blurb();
	Blurb C = Blurb();

	A = Blurb(glm::vec3(2, 0, 0), 1);
	B = Blurb(glm::vec3(0, 0, 0), 2);
	C = Blurb(glm::vec3(-2, 0, 0), 3);

	/*A.SetShaderProgram(shader.GetProgram());
	A.SetTexture(texture);

	B.SetShaderProgram(shader.GetProgram());
	B.SetTexture(texture);

	C.SetShaderProgram(shader.GetProgram());
	C.SetTexture(texture);*/

	Blurbs.push_back(A);
	Blurbs.push_back(B);
	Blurbs.push_back(C);

	MapFactory MF = MapFactory();
	MF.LoadMap();

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
	glClearColor(0.5f, 0.2f, 0.7f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i].Draw();
	}
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





