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
	~Blurb();

	void Buffer();
	void Init();
	void Update();
	void SetUniforms();
	void UpdateModelMatrix();

	void Draw();


	bool Constructed = false;

	string Name;
	int ID = 0;

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	bool isInit;

	string textureFile;
	GLuint textureID;

	string normalFile;
	GLuint normalID;

	GLfloat * VertArray;

	mat4 normalMatrix;
	mat4 model;

	Shader shader;
	GLuint shaderProgram;

	bool Buffered;

	GLuint VAO, VBO;
};

