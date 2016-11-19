#pragma once
#include "Shader.h"
#include "GameObject.h"

#include "..\include\math\vect3d.h"
#include "..\include\GL\glew.h"
#include "..\include\glm\glm.hpp"

#include <string>

using namespace std;
using namespace glm;

class Billboard : public GameObject
{

public:
	Billboard();
	Billboard(vec3 _pos, int _mode);
	~Billboard();

	void Buffer();
	void Init();
	void Update();
	void SetUniforms();
	void UpdateModelMatrix();

	void Draw();


	vec3 Origin = vec3(0, 0, 0);

	bool Constructed = false;
	string Name;
	int ID = 0;

	vec3 Position;
	vec3 Rotation;
	vec3 Scale;

	bool isInit;

	string textureFile;
	GLuint textureID;

	GLfloat * VertArray;

	mat4 model;

	mat4 normalMatrix;

	Shader shader;
	GLuint shaderProgram;

	bool Buffered;

	GLuint VAO, VBO;
};

