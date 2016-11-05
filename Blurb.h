#pragma once

#include "..\include\math\vect3d.h"
#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"

#include <string>

using namespace std;
using namespace glm;

class Blurb
{
public:
	Blurb();
	Blurb(int _mode);
	void SetShaderProgram(GLuint Program);
	GLuint GetShaderProgram();
	void Buffer();
	void SetUniforms();
	void SetTexture(GLuint Texture);
	void Draw();
	~Blurb();

	float mode = 0;
	vec3 position;
	string objName;
	GLfloat * VertArray;
	GLint texture;
	GLuint shaderProgram;
	GLuint VBO, VAO, EBO;
protected:

private:

};

