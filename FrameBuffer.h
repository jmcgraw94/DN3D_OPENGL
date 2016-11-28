#pragma once

#include "..\include\glm\glm.hpp"
#include "..\include\GL\glew.h"

#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"
#include "..\include\glfw3.h"
#include "ShaderPipeline.h"

class FrameBuffer
{
public:
	FrameBuffer();
	~FrameBuffer();

	GLuint RenderBufferObject;
	GLuint FrameBufferObject;
	GLuint TextureColorbuffer;
	ShaderPipeline ScreenShader;
	GLuint quadVAO, quadVBO;
	GLuint texture;

	GLfloat quadVertices[36] = {
		-1.0f,  1.0f,  0.0f, 1.0f,
		-1.0f, -1.0f,  0.0f, 0.0f,
		1.0f, -1.0f,  1.0f, 0.0f,

		-1.0f,  1.0f,  0.0f, 1.0f,
		1.0f, -1.0f,  1.0f, 0.0f,
		1.0f,  1.0f,  1.0f, 1.0f
	};

	GLuint generateAttachmentTexture();

	void Dispose();

	void BindFrameBuffer();

	void CleanseBuffer();

	void DrawFrameBuffer();

	void Setup();



};

