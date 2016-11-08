#pragma once
#include "Shader.h"

#include "..\include\math\vect3d.h"
#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"

#include <string>

using namespace std;
using namespace glm;

class GameObject
{
public:
	GameObject();
	~GameObject();

	void SetShaderProgram(GLuint Program);
	GLuint GetShaderProgram();
	void Buffer();
	void Update();
	void SetUniforms();
	void UpdateModelMatrix();
	void SetTexture(GLuint Texture);
	void Draw();

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;

	Shader shader;

	float ID = 0;
	string objName;
	GLfloat * VertArray;
	GLuint texture;
	GLuint shaderProgram;
	GLuint VBO, VAO, EBO;
};
