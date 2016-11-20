#include "Main.h"

#include "..\include\SOIL.h"
#include "..\include\math\matrix4d.h"
#include "..\include\math\vect3d.h"
#include "..\include\glfw3.h"
#include "..\include\glm\glm.hpp"
#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"
#include "..\include\MapFactory.h"
#include "Texture2D.h"
#include "Billboard.h"
#include "PointLight.h"

#include <glew.h>
#include <stdlib.h>
#include <time.h>
#include <GL\glut.h>
#include <GL\GL.h>
#include <vector>
#include <stdio.h>
#include <string>
#include <iostream>
#include <algorithm>


using namespace std;
using namespace glm;

//Extern
int WIN_W = 1200;
int WIN_H = 800;
float PI = 3.141592654f;

//Static
int Main::FrameCount = 0;
bool Main::HeldKeys[1024];
bool Main::TapKeys[1024];
GLFWwindow * Main::window;
Camera Main::MainCamera;
ContentManager Main::CM = ContentManager();
MapFactory Main::MF = MapFactory();
vec2 Main::MousePos, Main::OldMousePos, Main::DeltaMousePos, Main::QuadraticOldMousePos, Main::QuadraticMousePos,
Main::QuadraticDeltaMousePos, Main::Q_Delta;

double Main::Time, Main::OldTime, Main::DeltaTime;

Texture2D Main::Map;

//Private
vector<Blurb> Blurbs = vector<Blurb>();
vector<Billboard> Billboards = vector<Billboard>();
float FrameRate;

Blurb * GlowBlurb = new Blurb();
PointLight * Main::P_Light;

Billboard * Bill;

// Is called whenever a key is pressed/released via GLFW
void Main::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		if (!Main::HeldKeys[key]) {
			TapKeys[key] = true;
		}

		Main::HeldKeys[key] = true;
	}
	else if (action == GLFW_RELEASE)
		Main::HeldKeys[key] = false;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void Main::mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	MousePos.x = xpos;
	MousePos.y = ypos;
}

void Main::resize_callback(GLFWwindow * window, int x, int y) {
	WIN_W = x;
	WIN_H = y;

	cout << x << "," << y << endl;
}

void Main::Setup() {
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;

	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	Main::window = glfwCreateWindow(WIN_W, WIN_H, "RogueGL", nullptr, nullptr);
	glfwSetFramebufferSizeCallback(window, resize_callback);
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

	glfwSetCursorPos(window, WIN_W / 2, WIN_H / 2);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Map = Texture2D("Content/Map.png");

	GlowBlurb = new Blurb(vec3(2, 2, -2), 4);
	Blurbs.push_back(*GlowBlurb);

	P_Light = new PointLight(vec3(5, .5f, 5), vec3(1, 1, 1), 1.0f, 1.0f);

	Bill = new Billboard(vec3(3, -1, 5), 1);

	for (int y = 0; y < Map.height; y++) {
		for (int x = 0; x < Map.width; x++) {
			if (Map.GetPixel(x, y) == vec4(0, 0, 0, 1)) {
				Blurb B = Blurb(vec3(x, -1, y), 1);
				Blurbs.push_back(B);

				Blurb A = Blurb(vec3(x, 0, y), 1);
				Blurbs.push_back(A);

				//Blurb C = Blurb(vec3(x, 1, y), 1);
				//Blurbs.push_back(C);
			}
			else if (Map.GetPixel(x, y) == vec4(0, 1, 0, 1)) {
				Blurb B = Blurb(vec3(x, -1, y), 3);
				Blurbs.push_back(B);
			}
			else {

			}
			Blurb B = Blurb(vec3(x, -2, y), 2);
			Blurbs.push_back(B);

			//Blurb A = Blurb(vec3(x, 2, y), 4);
			//Blurbs.push_back(A);
		}
	}

	//glEnable(GL_FOG);
	//GL_FOG_START = 
	//cout << "VER: " << glGetString(GL_MAJOR_VERSION) << " ::" << glGetString(GL_MINOR_VERSION) << endl;
	//cout << "SETUP COMPLETE\n" << endl;
}

void Main::Update() {
	glfwPollEvents();

	P_Light->Update();

	int _rate = 5;
	if (Main::TapKeys[GLFW_KEY_R]) {
		cout << "BLURB SPAWNED" << endl;

		Blurb B = Blurb(
			vec3(
			(int)MainCamera.Position.x,
				(int)MainCamera.Position.y,
				(int)MainCamera.Position.z) + vec3(0, -1, 1),
			abs(FrameCount / _rate) % 3 + 1);

		Blurbs.push_back(B);
	}
	if (Main::TapKeys[GLFW_KEY_L]) {
		cout << "====== LOG ========" << endl;
		cout << "FPS: " << FrameRate << endl;
		cout << "BLURBS: " << Blurbs.size() << endl;
		cout << "====== --- ========" << endl;
	}
	if (Main::HeldKeys[GLFW_KEY_UP]) {
		Main::P_Light->Position.z -= .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_DOWN]) {
		Main::P_Light->Position.z += .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_LEFT]) {
		Main::P_Light->Position.x -= .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_RIGHT]) {
		Main::P_Light->Position.x += .1f;
	}
	if (Main::TapKeys[GLFW_KEY_COMMA]) {
		Main::P_Light->Color.r = Main::P_Light->Color.r != 0 ? 0 : 1;
	}
	if (Main::TapKeys[GLFW_KEY_PERIOD]) {
		Main::P_Light->Color.g = Main::P_Light->Color.g != 0 ? 0 : 1;
	}
	if (Main::TapKeys[GLFW_KEY_SLASH]) {
		Main::P_Light->Color.b = Main::P_Light->Color.b != 0 ? 0 : 1;
	}

	//	for (int x = -5; x < 5; x++) {
	//		for (int y = -5; y < 5; y++) {
	//			Blurb B = Blurb(vec3(x, -2, y), 1);
	//			Blurbs.push_back(B);
	//		}
	//	}

	//glutSetCursor(GLUT_CURSOR_NONE);
	//glfwDisable(GLFW_MOUSE_CURSOR);

	//cout << Blurbs.size() << endl;


	/*if (FrameCount % rate == 0)
	{
		Blurb B = Blurb(vec3(FrameCount / rate, abs(FrameCount / rate) % 3 + 1, 0), abs(FrameCount / rate) % 3 + 1);
		Blurbs.push_back(B);
	}*/

	/*for (int x = -5; x < 5; x++) {
		for (int y = -5; y < 5; y++) {
			Blurb B = Blurb(vec3(x, y, 0), abs(x) % 3 + 1);
			Blurbs.push_back(B);
			B.Init();
		}
	}*/


	Time = glfwGetTime();
	Main::MainCamera.Update();

	Bill->Update();

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i].Update();
		if (Blurbs[i].ID == 4)
			Blurbs[i].Position = Main::P_Light->Position - vec3(.5f, .5f, .5f);
	}
}

void Main::Draw() {
	//glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//BB.Draw();
	(*GlowBlurb).Position = Main::P_Light->Position;
	Bill->Draw();

	for (int i = 0; i < Blurbs.size(); i++) {
		//float Dist = Helper::Distance(MainCamera.Position, Blurbs[i].Position);
		Blurbs[i].Draw();
	}

	glDisable(GL_BLEND);
	glEnable(GL_DEPTH_TEST);
}

void Main::LateUpdate() {

	//for (int i = 0; i < Blurbs.size(); i++) {
	//	if (&Blurbs[i] == nullptr)
	//	{
	//		cout << i << endl;
	//	}
	//}

	FrameRate = (int)(1 / DeltaTime);
	glfwSwapBuffers(Main::window);
	DeltaTime = Time - OldTime;
	DeltaMousePos = MousePos - OldMousePos;

	QuadraticOldMousePos = QuadraticMousePos;
	QuadraticDeltaMousePos = (MousePos - QuadraticMousePos);
	QuadraticMousePos += QuadraticDeltaMousePos * 0.25f;
	Q_Delta = QuadraticMousePos - QuadraticOldMousePos;

	OldMousePos = MousePos;
	OldTime = Time;

	for (int i = 0; i < 1024; i++)
		TapKeys[i] = false;

	Main::FrameCount++;
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





