#include "Billboard.h"
#include "Main.h"

using namespace std;
using namespace glm;


Billboard::Billboard() {
	cout << "EMPTY BILLBOARD" << endl;
}

Billboard::Billboard(vec3 _pos, int _ID)
{
	Position = _pos;
	Scale = vec3(1, 1, 1);

	Dynamic = false;

	Constructed = true;

	ID = _ID;

	if (ID == 1)
		textureFile = "Content/tallgrass.png";
}

Billboard::~Billboard()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Billboard::Init() {
	if (!isInit) {
		isInit = true;

		Origin = vec3(-.5f, 0, 0);
		//Rotation.y = Helper::RandomRange(360);

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

		shader = Shader("Shaders/StandardVert.vert", "Shaders/DoubleSidedFrag.frag");
		shaderProgram = shader.GetProgram();
	}
}

void Billboard::Buffer() {

	GLfloat verts[] = {

		0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1, 0, 0, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1, 1, 0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		1, 1, 0, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0, 1, 0, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		0, 0, 0, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	};

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

void Billboard::SetUniforms() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, "MainTexture"), 0);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "normalMatrix"),
		1, GL_FALSE, glm::value_ptr(normalMatrix));

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

void Billboard::UpdateModelMatrix() {
	if (!Constructed)
		return;

	normalMatrix = mat4();
	normalMatrix = glm::rotate(normalMatrix, glm::radians(Rotation.z), vec3(0, 0, 1));
	normalMatrix = glm::rotate(normalMatrix, glm::radians(Rotation.y), vec3(0, 1, 0));
	normalMatrix = glm::rotate(normalMatrix, glm::radians(Rotation.x), vec3(1, 0, 0));

	model = mat4();
	model = glm::translate(model, Position);

	model = glm::rotate(model, glm::radians(Rotation.z), vec3(0, 0, 1));
	model = glm::rotate(model, glm::radians(Rotation.y), vec3(0, 1, 0));
	model = glm::rotate(model, glm::radians(Rotation.x), vec3(1, 0, 0));

	model = glm::translate(model, Origin);

	model = glm::scale(model, glm::vec3(Scale.x, Scale.y, Scale.z));


}

void Billboard::Update() {
	if (!Constructed)
		return;

	Init();

	//Blurb Apple = Blurb(); //Stack
	//Apple.ID = 10;

	//Blurb * Orange = new Blurb(); //Heap
	//Orange->ID = 15;
	if (Dynamic)
		Rotation.y = Helper::AngleBetween_DEG(
			vec2(Main::MainCamera.Position.x, Main::MainCamera.Position.z),
			vec2(Position.x, Position.z));
}

void Billboard::Draw() {
	if (!Constructed)
		return;

	UpdateModelMatrix();
	SetUniforms();

	Buffer();

	glDrawArrays(GL_TRIANGLES, 0, 6);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}




