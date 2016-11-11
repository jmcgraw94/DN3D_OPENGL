#include "Blurb.h"
#include "Main.h"
#include "ContentManager.h"
#include "MapFactory.h"
#include "Texture2D.h"

#include "..\include\SOIL.h"
#include "..\include\glfw3.h"
#include "..\include\GL\glew.h"
#include "..\include\math\matrix4d.h"
#include "..\include\math\vect3d.h"
#include "..\include\glm\glm.hpp"
#include "..\include\glfw3.h"
#include "..\include\glm\gtc\matrix_transform.hpp"
#include "..\include\glm\gtc\type_ptr.hpp"

#include <stdlib.h>
#include <GL\glut.h>
#include <GL\GL.h>
#include <time.h>
#include <vector>
#include <string>

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;
using namespace glm;


Blurb::Blurb() {
	cout << "EMPTY BLURB" << endl;
}

Blurb::Blurb(vec3 _pos, int _ID)
{
	Position = _pos;
	Scale = vec3(1, 1, 1);

	ID = _ID;

	if (ID == 1)
		textureFile = "Content/brick.png";
	if (ID == 2)
		textureFile = "Content/planks_birch.png";
	if (ID == 3)
		textureFile = "Content/emerald_block.png";
}

Blurb::~Blurb()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Blurb::Init() {
	if (!isInit) {
		isInit = true;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		Texture2D texture = Texture2D(textureFile);

		//float t_GridTexture[16] = {
		//	1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 1.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
		//};

		//Define texture images
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, t_GridTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.width, texture.height, 0, GL_RGBA, GL_FLOAT, texture.Pixels);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the filter parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		shader = Shader("Shaders/StandardVert.vert", "Shaders/StandardFrag.frag");
		shaderProgram = shader.GetProgram();

	}
}

void Blurb::Buffer() {

	GLfloat verts[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

		0.5f,  0.5f,  0.5f, 1.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		0.5f, -0.5f,  0.5f, 1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 1.0f,

		//Top
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		//Bottom
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	//---------------------------------

	if (!Buffered) {
		Buffered = true;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
	}

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 *
		sizeof(GLfloat), (GLvoid*)0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 *
		sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0); // Position attribute	  
	glEnableVertexAttribArray(1); // TexCoord attribute

}

void Blurb::SetUniforms() {
	glUseProgram(shaderProgram);

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Main::MainCamera.view));

	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Main::MainCamera.projection));

}

void Blurb::UpdateModelMatrix() {
	model = mat4();
	model = glm::translate(model, Position);

	model = glm::rotate(model, glm::radians(Rotation.z), vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(Rotation.y), vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(Rotation.x), vec3(1, 0, 0));

	model = glm::scale(model, glm::vec3(Scale.x, Scale.y, Scale.z));
}

void Blurb::Update() {
	Init();
	//Rotation.x += 1.0f;
	//Rotation.z += 1.0f;
}

void Blurb::Draw() {
	UpdateModelMatrix();
	SetUniforms();

	Buffer();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}




