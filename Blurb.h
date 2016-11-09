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
	//void SetShaderProgram(GLuint Program);
	//GLuint GetShaderProgram();
	void Buffer();
	void Init();
	void Update();
	void SetUniforms();
	void UpdateModelMatrix();
	//void SetTexture(GLuint Texture);
	void Draw();
	//int LoadGLTextures();
	~Blurb();


	string Name;
	float ID = 0;

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	bool isInit;

	string textureFile;
	GLuint textureID;

	GLfloat * VertArray;

	mat4 model;
	
	Shader shader;
	GLuint shaderProgram;

	GLuint VBO, VAO;

};

