#include "Blurb.h"
#include "Main.h"


using namespace std;
using namespace glm;

GLfloat verts[] = {
	//Back Face
	0, 0, 0,  0.0f,  0.0f,  0.0f, 0.0f,	-1.0f,
	1, 0, 0,  1.0f,  0.0f,  0.0f, 0.0f,	-1.0f,
	1, 1, 0,  1.0f,  1.0f,  0.0f, 0.0f,	-1.0f,
	1, 1, 0,  1.0f,  1.0f,  0.0f, 0.0f,	-1.0f,
	0, 1, 0,  0.0f,  1.0f,  0.0f, 0.0f,	-1.0f,
	0, 0, 0,  0.0f,  0.0f,  0.0f, 0.0f,	-1.0f,

	//Front Face
	0, 0,  1,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f,
	1, 0,  1,  1.0f,  0.0f, 0.0f, 0.0f, 1.0f,
	1, 1,  1,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	1, 1,  1,  1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	0, 1,  1,  0.0f,  1.0f, 0.0f, 0.0f, 1.0f,
	0, 0,  1,  0.0f,  0.0f, 0.0f, 0.0f, 1.0f,

	//Left Face
	0, 1,  1, 1.0f, 1.0f, -1.0f,  0.0f, 0.0f,
	0, 1,  0, 0.0f, 1.0f, -1.0f,  0.0f, 0.0f,
	0, 0,  0, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
	0, 0,  0, 0.0f, 0.0f, -1.0f,  0.0f, 0.0f,
	0, 0,  1, 1.0f, 0.0f, -1.0f,  0.0f, 0.0f,
	0, 1,  1, 1.0f, 1.0f, -1.0f,  0.0f, 0.0f,

	//Right Face
	1,  1,  1, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1,  1,  0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
	1,  0,  0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1,  0,  0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1,  0,  1, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	1,  1,  1, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

	//Top Face						 
	0, 0, 0,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	1, 0, 0,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
	1, 0, 1,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	1, 0, 1,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	0, 0, 1,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
	0, 0, 0,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

	//Bottom Face					  
	0,  1, 0,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1,  1, 0,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	1,  1,  1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	1,  1,  1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0,  1,  1, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	0,  1, 0,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
};

Blurb::Blurb() {
	cout << "EMPTY BLURB" << endl;
}

Blurb::Blurb(vec3 _pos, int _ID)
{
	Position = _pos;
	Scale = vec3(1, 1, 1);

	Constructed = true;

	ID = _ID;

	if (ID == 1)
		textureFile = "Content/brick.png";
	if (ID == 2)
		textureFile = "Content/planks_birch.png";
	if (ID == 3)
		textureFile = "Content/emerald_block.png";
	if (ID == 4)
		textureFile = "Content/planks_oak.png";
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
		//cout << (int)textureID << endl; //Texture ID can be shared?
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



	//---------------------------------

	if (!Buffered && Constructed) {
		Buffered = true;
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
	}



	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

	GLsizei stride = 8;

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride *
		sizeof(GLfloat), (GLvoid*)0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride *
		sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride *
		sizeof(GLfloat), (GLvoid*)(5 * sizeof(GLfloat)));

	glEnableVertexAttribArray(0); // Position attribute	  
	glEnableVertexAttribArray(1); // TexCoord attribute
	glEnableVertexAttribArray(2); // Normal attribute
}

void Blurb::SetUniforms() {
	glUseProgram(shaderProgram);	

	//for (int i = 5; i < 36; i += 8) {
	//	vec4 newNorm = vec4(verts[i], verts[i + 1], verts[i + 2], 1);
	//	newNorm = newNorm * model * view;
	//	verts[i + 0] = newNorm.x;
	//	verts[i + 1] = newNorm.y;
	//	verts[i + 2] = newNorm.z;
	//}

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
		1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
		1, GL_FALSE, glm::value_ptr(Main::MainCamera.view));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
		1, GL_FALSE, glm::value_ptr(Main::MainCamera.projection));

	glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"),
		Main::lightPos.x, Main::lightPos.y, Main::lightPos.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"),
		Main::lightColor.x, Main::lightColor.y, Main::lightColor.z);


}

void Blurb::UpdateModelMatrix() {
	if (!Constructed)
		return;

	model = mat4();
	model = glm::translate(model, Position);


	model = glm::rotate(model, glm::radians(Rotation.z), vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(Rotation.y), vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(Rotation.x), vec3(1, 0, 0));

	model = glm::scale(model, glm::vec3(Scale.x, Scale.y, Scale.z));
}

void Blurb::Update() {
	if (!Constructed)
		return;

	Init();

	Rotation.y += 0.1f;
	//Rotation.x += 1.0f;
	//Rotation.z += 1.0f;
}

void Blurb::Draw() {
	if (!Constructed)
		return;

	UpdateModelMatrix();
	SetUniforms();

	Buffer();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}




