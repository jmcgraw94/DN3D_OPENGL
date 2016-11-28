#pragma once

#include "Camera.h"
#include "Blurb.h"
#include "MapFactory.h"
#include "ContentManager.h"
#include "Texture2D.h"
#include "ShaderPipeline.h"
#include "Camera.h"
#include "Helper.h"
#include "Texture2D.h"
#include "PointLight.h"
#include "FrameBuffer.h"

#include "..\include\glm\glm.hpp"
#include "..\include\GL\glew.h"

#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"
#include "..\include\glfw3.h"

#include <string>

extern int WINW, WINH;
extern float PI;

class Main
{
public:

	static int FrameCount;
	static bool HeldKeys[1024];
	static bool TapKeys[1024];
	static GLFWwindow* window;
	static Camera MainCamera;
	static ContentManager CM;
	static MapFactory MF;

	static Texture2D Map;

	static vector<PointLight> PointLights;

	static FrameBuffer FrameBufferObject;

	static PointLight 
		* P_Light1, 
		* P_Light2;


	static vec2
		MousePos,
		OldMousePos,
		DeltaMousePos,

		QuadraticMousePos,
		QuadraticOldMousePos,
		Q_Delta,
		QuadraticDeltaMousePos;

	static double
		Time,
		OldTime,
		DeltaTime;

	static void key_callback(GLFWwindow * window, int key, int scancode, int action, int mode);
	static void mouse_callback(GLFWwindow * window, double xpos, double ypos);
	static void resize_callback(GLFWwindow * window, int x, int y);

	static void Setup();
	static void Update();
	static void Draw();
	static void LateUpdate();
};

