#include "Billboard.h"
#include "Main.h"

using namespace std;
using namespace glm;


Blurb::Blurb() {
	cout << "EMPTY BILLBOARD" << endl;
}

Blurb::Blurb(vec3 _pos, int _ID)
{
	Position = _pos;
	Scale = vec3(1, 1, 1);

	//Dynamic = true;

	Constructed = true;

	ID = _ID;

	if (ID == 1)
		textureFile = "Content/brick.png";
	if (ID == 2)
		textureFile = "Content/planks_birch.png";
	if (ID == 3)
		textureFile = "Content/emerald_block.png";
	if (ID == 4)
		textureFile = "Content/white.png";

	normalFile = "Content/brick_normal.png";
}

Blurb::~Blurb()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Blurb::Init() {
	if (!isInit) {
		isInit = true;


		Origin = vec3(-.5f, 0, 0);
		//Rotation.y = Helper::RandomRange(360);

		GLchar * VertexShaderPath = "Shaders/StandardVert.vert";
		GLchar * FragmentShaderPath = "Shaders/StandardFrag.frag";

		if (ID == 4) {
			FragmentShaderPath = "Shaders/UnlitFrag.frag";
			Scale = vec3(.25f, .25f, .25f);
			Origin = vec3(1, 1, 1) * .5f;
		}

		shader = ShaderPipeline(VertexShaderPath, FragmentShaderPath);
		shaderProgram = shader.GetProgram();

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

		//shader = Shader("Shaders/StandardVert.vert", "Shaders/StandardFrag.frag");
		//shaderProgram = shader.GetProgram();
	}
}

void Blurb::Buffer() {

	GLfloat verts[] = {
		//Back Face
		1, 1, 0,  1.0f,  1.0f,  0.0f, 0.0f,	-1.0f,
		1, 0, 0,  1.0f,  0.0f,  0.0f, 0.0f,	-1.0f,
		0, 0, 0,  0.0f,  0.0f,  0.0f, 0.0f,	-1.0f,
		0, 0, 0,  0.0f,  0.0f,  0.0f, 0.0f,	-1.0f,
		0, 1, 0,  0.0f,  1.0f,  0.0f, 0.0f,	-1.0f,
		1, 1, 0,  1.0f,  1.0f,  0.0f, 0.0f,	-1.0f,

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
		1,  0,  0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1,  1,  0, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1,  1,  1, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1,  1,  1, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,
		1,  0,  1, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		1,  0,  0, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

		//Bottom Face						 
		0, 0, 0,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		1, 0, 0,  1.0f, 1.0f, 0.0f, -1.0f, 0.0f,
		1, 0, 1,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		1, 0, 1,  1.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0, 0, 1,  0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
		0, 0, 0,  0.0f, 1.0f, 0.0f, -1.0f, 0.0f,

		//Top Face					  
		1,  1,  1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1,  1, 0,  1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0,  1, 0,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0,  1, 0,  0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		0,  1,  1, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f,
		1,  1,  1, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
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


void Blurb::SetUniforms() {
	glUseProgram(shaderProgram);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glUniform1i(glGetUniformLocation(shaderProgram, "MainTexture"), 0);

	if (ID == 4) {
		glUniform3f(glGetUniformLocation(shaderProgram, "Tint"),
			Main::P_Light1->Color.r, Main::P_Light1->Color.b, Main::P_Light1->Color.g);
	}

	//glActiveTexture(GL_TEXTURE1);
	//glBindTexture(GL_TEXTURE_2D, normalID);
	//glUniform1i(glGetUniformLocation(shaderProgram, "NormalTexture"), 1);

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "normalMatrix"),
		1, GL_FALSE, glm::value_ptr(normalMatrix));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
		1, GL_FALSE, glm::value_ptr(model));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
		1, GL_FALSE, glm::value_ptr(Main::MainCamera.view));

	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
		1, GL_FALSE, glm::value_ptr(Main::MainCamera.projection));

	glUniform1i(glGetUniformLocation(shaderProgram, "LightCount"), Main::PointLights.size());

	glUniform1i(glGetUniformLocation(shaderProgram, "DSL"), 1);
	glUniform1i(glGetUniformLocation(shaderProgram, "DistanceLighting"), 1);
	/*for (int i = 0; i < Main::PointLights.size(); i++) {
	string _i = std::to_string(i);

	glUniform3f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Position").c_str()),
	Main::PointLights[i].Position.x, Main::PointLights[i].Position.y, Main::PointLights[i].Position.z);

	glUniform3f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Color").c_str()),
	Main::PointLights[i].Color.x, Main::PointLights[i].Color.y, Main::PointLights[i].Color.z);

	glUniform1f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Range").c_str()),
	Main::PointLights[i].Range);

	glUniform1f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Brightness").c_str()),
	Main::PointLights[i].Brightness);
	}*/
	// ----

	glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[0].Position"),
		Main::P_Light1->Position.x, Main::P_Light1->Position.y, Main::P_Light1->Position.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[0].Color"),
		Main::P_Light1->Color.x, Main::P_Light1->Color.y, Main::P_Light1->Color.z);

	glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[0].Range"),
		Main::P_Light1->Range);

	glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[0].Brightness"),
		Main::P_Light1->Brightness);

	//// ------------

	glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[1].Position"),
		Main::P_Light2->Position.x, Main::P_Light2->Position.y, Main::P_Light2->Position.z);

	glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[1].Color"),
		Main::P_Light2->Color.x, Main::P_Light2->Color.y, Main::P_Light2->Color.z);

	glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[1].Range"),
		Main::P_Light2->Range);

	glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[1].Brightness"),
		Main::P_Light2->Brightness);
}
//void Billboard::SetUniforms() {
//	glUseProgram(shaderProgram);
//
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, textureID);
//	glUniform1i(glGetUniformLocation(shaderProgram, "MainTexture"), 0);
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "normalMatrix"),
//		1, GL_FALSE, glm::value_ptr(normalMatrix));
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
//		1, GL_FALSE, glm::value_ptr(model));
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
//		1, GL_FALSE, glm::value_ptr(Main::MainCamera.view));
//
//	glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
//		1, GL_FALSE, glm::value_ptr(Main::MainCamera.projection));
//
//	glUniform1i(glGetUniformLocation(shaderProgram, "LightCount"), Main::PointLights.size());
//
//	glUniform1f(glGetUniformLocation(shaderProgram, "DSL"), 1);
//
//	glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"),
//		Main::P_Light1->Position.x, Main::P_Light1->Position.y, Main::P_Light1->Position.z);
//
//	glUniform3f(glGetUniformLocation(shaderProgram, "lightColor"),
//		Main::P_Light1->Color.x, Main::P_Light1->Color.y, Main::P_Light1->Color.z);
//
//	//glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[0].Position"),
//	//	Main::P_Light1->Position.x, Main::P_Light1->Position.y, Main::P_Light1->Position.z);
//
//	//glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[0].Color"),
//	//	Main::P_Light1->Color.x, Main::P_Light1->Color.y, Main::P_Light1->Color.z);
//
//	//glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[0].Range"),
//	//	Main::P_Light1->Range);
//
//	//glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[0].Brightness"),
//	//	Main::P_Light1->Brightness);
//
//
//
//}

void Blurb::UpdateModelMatrix() {
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

void Blurb::Update() {
	if (!Constructed)
		return;

	Init();

	//if (Dynamic)
	//	Rotation.y = Helper::AngleBetween_DEG(
	//		vec2(Main::MainCamera.Position.x, Main::MainCamera.Position.z),
	//		vec2(Position.x, Position.z));
}

void Blurb::Draw() {
	if (!Constructed)
		return;

	UpdateModelMatrix();
	SetUniforms();

	Buffer();

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}




