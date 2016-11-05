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


int W_WIDTH = 800;
int W_HEIGHT = 600;

bool Main::keys[1024];

GLFWwindow * window;
vec2 MousePos, OldMousePos;

Camera MainCamera = Camera();
Blurb Main::A = Blurb();
Blurb Main::B = Blurb();

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode)
{
	if (action == GLFW_PRESS)
		Main::keys[key] = true;
	else if (action == GLFW_RELEASE)
		Main::keys[key] = false;

	//cout << key << endl;

	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}
void mouse_callback(GLFWwindow * window, double xpos, double ypos) {
	MousePos.x = xpos;
	MousePos.y = ypos;

	//cout << MousePos.x << "," << MousePos.y << endl;
}


// The MAIN function, from here we start the application and run the game loop
int main()
{
	std::cout << "Starting GLFW context, OpenGL 3.3" << std::endl;
	// Init GLFW
	glfwInit();

	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	GLFWwindow* window = glfwCreateWindow(W_WIDTH, W_HEIGHT, "LearnOpenGL", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);
	glfwSetCursorPosCallback(window, mouse_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;

	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	int _w, _h;
	glfwGetFramebufferSize(window, &_w, &_h);
	glViewport(0, 0, _w, _h);

	Shader shader = Shader("Shaders/VertexShader.vert", "Shaders/FragShader.frag");

	GLuint texture;
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture); // All upcoming GL_TEXTURE_2D operations now have effect on this texture object

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
	int texWidth, texHeight;
	unsigned char* image = SOIL_load_image("Content/container.png", &texWidth, &texHeight, 0, SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texWidth, texHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
	glGenerateMipmap(GL_TEXTURE_2D);
	SOIL_free_image_data(image);
	glBindTexture(GL_TEXTURE_2D, 0);

	glEnable(GL_DEPTH_TEST);

	Main::A = Blurb(-1);
	cout << Main::A.mode << endl;

	Main::B = Blurb();
	cout << Main::B.mode << endl;


	MapFactory MF = MapFactory();
	MF.LoadMap();

	// Game loop
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();

		MainCamera.Update();

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		

		GLint viewLoc = glGetUniformLocation(Main::A.GetShaderProgram(), "view");
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.view));

		GLint projLoc = glGetUniformLocation(Main::A.GetShaderProgram(), "projection");
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(MainCamera.projection));

		Main::A.SetShaderProgram(shader.GetProgram());
		Main::A.SetTexture(texture);
		Main::B.SetShaderProgram(shader.GetProgram());
		Main::B.SetTexture(texture);

		Main::A.Draw();
		Main::B.Draw();

		glfwSwapBuffers(window);
		OldMousePos = MousePos;
	}
	glfwTerminate();
	return 0;
}





