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
int WINW = 1200;
int WINH = 800;
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
FrameBuffer Main::ScreenFBO;
vector<PointLight * > Main::PointLights;
int Main::ColorBitDepth = 24;
double Main::StartFrameTime, Main::EndFrameTime, Main::DeltaTime;
Texture2D Main::Map;

//Private
vector<Blurb * > Blurbs = vector<Blurb * >();
vector<Billboard> Billboards = vector<Billboard>();
float FrameRate;

Blurb * GlowBlurb1 = new Blurb();
Blurb * GlowBlurb2 = new Blurb();

Billboard * Bill1;
Billboard * Bill2;

FrameBuffer GeometryFrameBuffer;

void Main::key_callback(GLFWwindow * window, int key, int scancode, int action, int mode) {
	if (action == GLFW_PRESS) {
		if (!Main::HeldKeys[key]) {
			TapKeys[key] = true;
		}

		Main::HeldKeys[key] = true;
	}
	else if (action == GLFW_RELEASE)
		Main::HeldKeys[key] = false;

}

void Main::mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	MousePos.x = xpos;
	MousePos.y = ypos;
}

void Main::resize_callback(GLFWwindow * window, int x, int y) {
	WINW = x;
	WINH = y;

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
	Main::window = glfwCreateWindow(WINW, WINH, "RogueGL", nullptr, nullptr);
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


	glfwSetCursorPos(window, WINW / 2, WINH / 2);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Map = Texture2D("Content/Map.png");

	GlowBlurb1 = new Blurb(vec3(2, 4, -2), 4);
	Blurbs.push_back(GlowBlurb1);

	GlowBlurb2 = new Blurb(vec3(2, 4, -2), 4);
	Blurbs.push_back(GlowBlurb2);

	PointLight * P_Light1 = new PointLight(vec3(05, 2.5f, 5), vec3(1, 1, 1), 6.0f, 1.0f);
	PointLight * P_Light2 = new PointLight(vec3(10, 2.5f, 4), vec3(0, 1, 1), 6.0f, 1.0f);
	PointLight * P_Light3 = new PointLight(vec3(15, 2.5f, 6), vec3(1, 1, 1), 6.0f, 1.0f);

	PointLights.push_back(P_Light1);
	PointLights.push_back(P_Light2);
	PointLights.push_back(P_Light3);
	
	Bill1 = new Billboard(vec3(3, 1, 5), 3);
	Bill2 = new Billboard(vec3(1.15f, 2.25f, 6), 2);

	MousePos = vec2(WINW, WINH) / 2.0f;
	OldMousePos = vec2(WINW, WINH) / 2.0f;

	for (int y = 0; y < Map.height; y++) {
		for (int x = 0; x < Map.width; x++) {
			if (Map.GetPixel(x, y) == vec4(0, 0, 0, 1)) {
				Blurb * B = new Blurb(vec3(x, 1, y), 7);
				Blurbs.push_back(B);

				Blurb * A = new Blurb(vec3(x, 2, y), 1);
				Blurbs.push_back(A);

				Blurb * C = new Blurb(vec3(x, 3, y), 1);
				Blurbs.push_back(C);
			}
			else if (Map.GetPixel(x, y) == vec4(0, 1, 0, 1)) {
				Blurb * B = new Blurb(vec3(x, 1, y), 3);
				Blurbs.push_back(B);
			}
			else if (Map.GetPixel(x, y) == vec4(0, 0, 1, 1)) {
				Blurb * A = new Blurb(vec3(x, 1, y), 8);
				Blurbs.push_back(A);

				Blurb * B = new Blurb(vec3(x, 2, y), 5);
				Blurbs.push_back(B);

				Blurb * C = new Blurb(vec3(x, 3, y), 5);
				Blurbs.push_back(C);

			}
			else if (Map.GetPixel(x, y) == vec4(1, 0, 0, 1)) {
				Blurb * B = new Blurb(vec3(x, 0, y), 6);
				Blurbs.push_back(B);
			}
			else {
				Blurb * B = new Blurb(vec3(x, 0, y), 2);
				Blurbs.push_back(B);

				
			}
			Blurb * A = new Blurb(vec3(x, 4, y), 6);
			Blurbs.push_back(A);
		}
	}

	ScreenFBO.Setup();

	printf("%s\n", glGetString(GL_VERSION));
}

void Main::Update() {
	glfwPollEvents();

	for (int i = 0; i < PointLights.size(); i++) {
		PointLights[i]->Update();
	}


	int _rate = 5;
	if (Main::TapKeys[GLFW_KEY_R]) {
		cout << "BLURB SPAWNED" << endl;

		vec3 clampPos = vec3(
			(int)MainCamera.Position.x,
			(int)MainCamera.Position.y,
			(int)MainCamera.Position.z);

		PointLight * P = new PointLight(clampPos, vec3(1, 1, 1), 8.0f, 1.1f);
		PointLights.push_back(P);

		//Blurb * B = new Blurb(clampPos, 1);
		//Blurbs.push_back(B);
	}
	if (Main::TapKeys[GLFW_KEY_L]) {
		cout << "====== LOG ========" << endl;
		cout << "FPS: " << FrameRate << endl;
		cout << "BLURBS: " << Blurbs.size() << endl;
		cout << "COLOR DEPTH: " << ColorBitDepth / 3 << " bits" << " (" << ColorBitDepth << " bits total)" << endl;
		cout << "LIGHTS: " << PointLights.size() << endl;
		cout << "RENDER TIME: " << DeltaTime << "ms" << endl;
		cout << "====== --- ========" << endl;
	}
	if (Main::HeldKeys[GLFW_KEY_UP]) {
		Main::Main::PointLights[0]->Position.z -= .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_DOWN]) {
		Main::Main::PointLights[0]->Position.z += .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_LEFT]) {
		Main::Main::PointLights[0]->Position.x -= .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_RIGHT]) {
		Main::Main::PointLights[0]->Position.x += .1f;
	}
	if (Main::TapKeys[GLFW_KEY_COMMA]) {
		Main::Main::PointLights[0]->Color.r = Main::Main::PointLights[0]->Color.r != 0 ? 0 : 1;
	}
	if (Main::TapKeys[GLFW_KEY_PERIOD]) {
		Main::Main::PointLights[0]->Color.g = Main::Main::PointLights[0]->Color.g != 0 ? 0 : 1;
	}
	if (Main::TapKeys[GLFW_KEY_SLASH]) {
		Main::Main::PointLights[0]->Color.b = Main::Main::PointLights[0]->Color.b != 0 ? 0 : 1;
	}

	if (Main::TapKeys[GLFW_KEY_ESCAPE]) {
		glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
	}

	if (Main::TapKeys[GLFW_KEY_P]) {
		ColorBitDepth += 2;
	}
	if (Main::TapKeys[GLFW_KEY_O]) {
		ColorBitDepth -= 2;
	}

	StartFrameTime = glfwGetTime();
	Main::MainCamera.Update();

	Bill1->Update();
	Bill2->Update();

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i]->Update();
		if (Blurbs[i]->ID == 4)
			Blurbs[i]->Position = Main::Main::PointLights[0]->Position - vec3(.5f, .5f, .5f);
	}
}

void Main::Draw() {
	ScreenFBO.BindFrameBuffer();

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	GlowBlurb1->Position = Main::PointLights[0]->Position;
	GlowBlurb2->Position = Main::PointLights[1]->Position;

	Bill1->Draw();
	Bill2->Draw();

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i]->Draw();
	}

	glDisable(GL_BLEND);

	ScreenFBO.CleanseBuffer();
	ScreenFBO.DrawFrameBuffer();
}

void Main::LateUpdate() {
	glfwSwapBuffers(Main::window);

	FrameRate = (int)(1 / DeltaTime);
	DeltaTime = StartFrameTime - EndFrameTime;
	DeltaMousePos = MousePos - OldMousePos;

	QuadraticOldMousePos = QuadraticMousePos;
	QuadraticDeltaMousePos = (MousePos - QuadraticMousePos);
	QuadraticMousePos += QuadraticDeltaMousePos * 0.25f;
	Q_Delta = QuadraticMousePos - QuadraticOldMousePos;

	OldMousePos = MousePos;
	EndFrameTime = StartFrameTime;

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

	Main::ScreenFBO.Dispose();
	glfwTerminate();

	return 0;
}





