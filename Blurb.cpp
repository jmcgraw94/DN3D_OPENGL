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
	WaveDirection = vec3(1, 1, 1);
	ID = _ID;

	if (ID == 1)
		textureFile = "Content/brick.png";
	if (ID == 2)
		textureFile = "Content/planks_birch.png";
	if (ID == 3)
		textureFile = "Content/trapdoor.png";
	if (ID == 4)
		textureFile = "Content/white.png";
	if (ID == 5)
		textureFile = "Content/cobblestone_mossy.png";
	if (ID == 6)
		textureFile = "Content/trapdoor.png";
	if (ID == 7)
		textureFile = "Content/bookshelf.png";
	if (ID == 8)
		textureFile = "Content/stonebrick.png";
	if (ID == 9) {
		textureFile = "Content/water.png";
		WaveDirection = vec3(0, 1, 0);
	}

	normalFile = "Content/emerald_block_spec.png";
}

Blurb::~Blurb()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Blurb::Init() {
	if (!isInit) {
		isInit = true;


		Origin = vec3(0, 0, 0);
		/*TextureTrans = vec2(1, 1);*/

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

		float t_GridTexture[16] = {
			1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 1.0f,
			0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
		};

		//Define texture images
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, t_GridTexture);

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
			Main::PointLights[0]->Color.r, Main::PointLights[0]->Color.g, Main::PointLights[0]->Color.b);
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

	glUniform1i(glGetUniformLocation(shaderProgram, "DSL"), 0);

	glUniform1i(glGetUniformLocation(shaderProgram, "DistanceLighting"), 1);

	glUniform1f(glGetUniformLocation(shaderProgram, "Time"), Main::TotalTime);

	if (ID == 9)
		glUniform1f(glGetUniformLocation(shaderProgram, "WaveFactor"), clamp(abs((float)sin(Main::TotalTime / 10)),0.5f,1.0f));
	else
		glUniform1f(glGetUniformLocation(shaderProgram, "WaveFactor"), Main::WaveFactor);

	glUniform3f(glGetUniformLocation(shaderProgram, "WaveDirection"),
		WaveDirection.x - Origin.x, WaveDirection.y - Origin.y, WaveDirection.z - Origin.z);

	glUniform2f(glGetUniformLocation(shaderProgram, "TextureTrans"),
		TextureTrans.x, TextureTrans.y);

	glUniform3f(glGetUniformLocation(shaderProgram, "WorldPos"),
		Position.x - Origin.x, Position.y - Origin.y, Position.z - Origin.z);

	for (int i = 0; i < Main::PointLights.size(); i++) {
		string _i = std::to_string(i);
		//cout << _i << endl;

		glUniform3f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Position").c_str()),
			Main::PointLights[i]->Position.x, Main::PointLights[i]->Position.y, Main::PointLights[i]->Position.z);

		glUniform3f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Color").c_str()),
			Main::PointLights[i]->Color.x, Main::PointLights[i]->Color.y, Main::PointLights[i]->Color.z);

		glUniform1f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Range").c_str()),
			Main::PointLights[i]->Range);

		glUniform1f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Brightness").c_str()),
			Main::PointLights[i]->Brightness);
	}
	// ----

	//glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[0].Position"),
	//	Main::P_Light1->Position.x, Main::P_Light1->Position.y, Main::P_Light1->Position.z);

	//glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[0].Color"),
	//	Main::P_Light1->Color.x, Main::P_Light1->Color.y, Main::P_Light1->Color.z);

	//glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[0].Range"),
	//	Main::P_Light1->Range);

	//glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[0].Brightness"),
	//	Main::P_Light1->Brightness);

	//// ------------

	//glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[1].Position"),
	//	Main::P_Light2->Position.x, Main::P_Light2->Position.y, Main::P_Light2->Position.z);

	//glUniform3f(glGetUniformLocation(shaderProgram, "PointLights[1].Color"),
	//	Main::P_Light2->Color.x, Main::P_Light2->Color.y, Main::P_Light2->Color.z);

	//glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[1].Range"),
	//	Main::P_Light2->Range);

	//glUniform1f(glGetUniformLocation(shaderProgram, "PointLights[1].Brightness"),
	//	Main::P_Light2->Brightness);
}

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

	if (ID == 9)
		TextureTrans += vec2(Main::DeltaTime, Main::DeltaTime) / 2.0f;

	//if (Dynamic)
		//Rotation.y = Helper::AngleBetween_DEG(
		//	vec2(Main::MainCamera.Position.x, Main::MainCamera.Position.z),
		//	vec2(Position.x, Position.z));
}

void Blurb::Draw() {
	if (!Constructed)
		return;

	UpdateModelMatrix();
	SetUniforms();

	Buffer();

	glDrawArrays(GL_TRIANGLES, 0, 36);

	//glDrawArraysInstanced(GL_TRIANGLES, 0, 1, 36);

	//glDisableVertexAttribArray(0);
	//glDisableVertexAttribArray(1);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);
	//glBindTexture(GL_TEXTURE_2D, 0);
}




