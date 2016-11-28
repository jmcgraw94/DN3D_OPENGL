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

vector<PointLight> Main::PointLights;

double Main::Time, Main::OldTime, Main::DeltaTime;

Texture2D Main::Map;

//Private
vector<Blurb> Blurbs = vector<Blurb>();
vector<Billboard> Billboards = vector<Billboard>();
float FrameRate;

Blurb * GlowBlurb1 = new Blurb();
Blurb * GlowBlurb2 = new Blurb();

PointLight * Main::P_Light1;
PointLight * Main::P_Light2;

Billboard * Bill1;
Billboard * Bill2;

//Framebuffer Stuff
GLuint rbo;
GLuint framebuffer;
GLuint textureColorbuffer;
Shader ScreenShader;
GLuint quadVAO, quadVBO;
GLuint texture;

GLfloat quadVertices[] = {   // Vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
							 // Positions   // TexCoords
	-1.0f,  1.0f,  0.0f, 1.0f,
	-1.0f, -1.0f,  0.0f, 0.0f,
	1.0f, -1.0f,  1.0f, 0.0f,

	-1.0f,  1.0f,  0.0f, 1.0f,
	1.0f, -1.0f,  1.0f, 0.0f,
	1.0f,  1.0f,  1.0f, 1.0f
};

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
	WINW = x;
	WINH = y;

	cout << x << "," << y << endl;
}

GLuint generateAttachmentTexture()
{
	//Generate texture ID and load texture data 
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINW, WINH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
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

	GlowBlurb1 = new Blurb(vec3(2, 2, -2), 4);
	Blurbs.push_back(*GlowBlurb1);

	GlowBlurb2 = new Blurb(vec3(2, 2, -2), 4);
	Blurbs.push_back(*GlowBlurb2);

	P_Light1 = new PointLight(vec3(5, .5f, 5), vec3(1, 1, 1), 8.0f, 1.0f);
	P_Light2 = new PointLight(vec3(10, .5f, 6), vec3(0, 1, 1), 3.0f, 1.0f);

	PointLights.push_back(*P_Light1);
	PointLights.push_back(*P_Light2);

	Bill1 = new Billboard(vec3(3, -1, 5), 1);
	Bill2 = new Billboard(vec3(5, -1, 5), 1);

	for (int y = 0; y < Map.height; y++) {
		for (int x = 0; x < Map.width; x++) {
			if (Map.GetPixel(x, y) == vec4(0, 0, 0, 1)) {
				Blurb B = Blurb(vec3(x, -1, y), 1);
				Blurbs.push_back(B);

				//Blurb A = Blurb(vec3(x, 0, y), 1);
				//Blurbs.push_back(A);

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

	GLchar * ScreenVertexShaderPath = "Shaders/ScreenVert.vert";
	GLchar * ScreenFragmentShaderPath = "Shaders/ScreenFrag.frag";
	ScreenShader = Shader(ScreenVertexShaderPath, ScreenFragmentShaderPath);


	// Framebuffers
	glGenFramebuffers(1, &framebuffer);
	glGenRenderbuffers(1, &rbo);
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);
	// Create a color attachment texture
	textureColorbuffer = generateAttachmentTexture();
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINW, WINH); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, rbo); // Now actually attach it
																								  // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);
}

void Main::Update() {
	glfwPollEvents();

	P_Light1->Update();
	P_Light2->Update();

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
		Main::P_Light1->Position.z -= .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_DOWN]) {
		Main::P_Light1->Position.z += .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_LEFT]) {
		Main::P_Light1->Position.x -= .1f;
	}
	if (Main::HeldKeys[GLFW_KEY_RIGHT]) {
		Main::P_Light1->Position.x += .1f;
	}
	if (Main::TapKeys[GLFW_KEY_COMMA]) {
		Main::P_Light1->Color.r = Main::P_Light1->Color.r != 0 ? 0 : 1;
	}
	if (Main::TapKeys[GLFW_KEY_PERIOD]) {
		Main::P_Light1->Color.g = Main::P_Light1->Color.g != 0 ? 0 : 1;
	}
	if (Main::TapKeys[GLFW_KEY_SLASH]) {
		Main::P_Light1->Color.b = Main::P_Light1->Color.b != 0 ? 0 : 1;
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

	Bill1->Update();
	Bill2->Update();

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i].Update();
		if (Blurbs[i].ID == 4)
			Blurbs[i].Position = Main::P_Light1->Position - vec3(.5f, .5f, .5f);
	}
}

void Main::Draw() {
	//glClearColor(0.2f, 0.4f, 0.6f, 1.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);

	//BB.Draw();
	(*GlowBlurb1).Position = Main::P_Light1->Position;
	//(*GlowBlurb2).Position = Main::P_Light2->Position;

	Bill1->Draw();
	Bill2->Draw();

	for (int i = 0; i < Blurbs.size(); i++) {
		Blurbs[i].Draw();
	}

	glDisable(GL_BLEND);
	 ////Clear all relevant buffers
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1.0f); // Set clear color to white (not really necessery actually, since we won't be able to see behind the quad anyways)
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST); // We don't care about depth information when rendering a single quad

	ScreenShader.Use();
	glUseProgram(ScreenShader.Program);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, textureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void Main::LateUpdate() {
	glfwSwapBuffers(Main::window);

	FrameRate = (int)(1 / DeltaTime);
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

	glDeleteFramebuffers(1, &framebuffer);

	glfwTerminate();

	return 0;
}





