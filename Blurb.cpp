#include "Blurb.h"
#include "Main.h"

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

#include <stdio.h>
#include <string>
#include <iostream>

using namespace std;
using namespace glm;

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

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};

GLuint indices[] = {  // Note that we start from 0!
	0, 1, 3,   // First Triangle
	1, 2, 3    // Second Triangle
};

Blurb::Blurb() {
	cout << "EMPTY BLURB" << endl;
}

Blurb::Blurb(vec3 _pos, int _mode)
{
	Position = _pos;
	Scale = vec3(2, 1, 1);
	Rotation = vec3(0, 0, 15);

	ID = _mode;

	cout << "NEW BLURB @X: " << Position.x << " @: " << ID << endl;
}

Blurb::~Blurb()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}


void Blurb::SetShaderProgram(GLuint _Program) {
	shaderProgram = _Program;
}

GLuint Blurb::GetShaderProgram()
{
	return shaderProgram;
}

void Blurb::SetTexture(GLuint _texture) {
	texture = _texture;
}

void Blurb::Buffer() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	//glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 *
		sizeof(GLfloat), (GLvoid*)0); //Verts
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //Color

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 5 *
		sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat))); //Texture

	glEnableVertexAttribArray(0); // Position attribute	  
								  //glEnableVertexAttribArray(1); // Color attribute
	glEnableVertexAttribArray(2); // TexCoord attribute

	glBindBuffer(GL_ARRAY_BUFFER, 0); // Note that this is allowed, the call to glVertexAttribPointer registered VBO as the currently bound vertex buffer object so afterwards we can safely unbind
	glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs), remember: do NOT unbind the EBO, keep it bound to this VAO

	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
}

void Blurb::SetUniforms() {
	mat4 model = mat4(1);

	model = glm::translate(model, Position);

	model = glm::rotate(model, glm::radians(Rotation.z) * ID, vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(Rotation.y) * ID, vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(Rotation.x) * ID, vec3(1, 0, 0));

	model = glm::scale(model, glm::vec3(Scale.x, Scale.y, Scale.z));

	cout << ID << ": " << Rotation.z << endl;

	GLint modelLoc = glGetUniformLocation(shaderProgram, "model");
	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

	GLint viewLoc = glGetUniformLocation(shaderProgram, "view");
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(Main::MainCamera.view));

	GLint projLoc = glGetUniformLocation(shaderProgram, "projection");
	glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(Main::MainCamera.projection));

	glUseProgram(shaderProgram);
}

void Blurb::UpdateModelMatrix() {
	

}

void Blurb::Update() {
	Position += vec3(.005f, 0, 0);
	//Rotation += vec3()
}

void Blurb::Draw() {
	glEnable(GL_BLEND);

	UpdateModelMatrix();
	
	Buffer();

	SetUniforms();

	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);

	glDisable(GL_BLEND);

}


