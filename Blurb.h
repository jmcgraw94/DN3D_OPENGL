#pragma once
#include "Shader.h"
#include "GameObject.h"

#include "..\include\math\vect3d.h"
#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"

#include <string>

using namespace std;
using namespace glm;

class Blurb : public GameObject
{

public:
	Blurb();
	Blurb(vec3 _pos, int _mode);
	void SetShaderProgram(GLuint Program);
	GLuint GetShaderProgram();
	void Buffer();
	void Init();
	void Update();
	void SetUniforms();
	void UpdateModelMatrix();
	void SetTexture(GLuint Texture);
	void Draw();
	~Blurb();

	glm::vec3 Position;
	glm::vec3 Rotation;
	glm::vec3 Scale;
	
	Shader shader;
	string textureFile;

	mat4 model;
	bool isInit = false;
	float ID = 0;
	string objName;
	GLfloat * VertArray;
	GLuint texture;
	unsigned char* rawTex;
	int texWidth, texHeight;
	GLuint shaderProgram;
	GLuint VBO, VAO, EBO;
};

