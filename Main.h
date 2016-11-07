#pragma once

#include "Camera.h"
#include "Blurb.h"
#include "ContentManager.h"

#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"
#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"
#include "..\include\glfw3.h"

#include <string>
extern int W_WIDTH, W_HEIGHT;

class Main
{
public:

	static bool keys[1024];
	static GLFWwindow* window;
	static Camera MainCamera;
	static ContentManager contentManager;

	static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow* window, double xpos, double ypos);
	
	static void Setup();
	static void Update();
	static void Draw();
	static void LateUpdate();

};

