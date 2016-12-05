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

	Dynamic = true;

	Constructed = true;

	AnimTimer = new AnimationTimer(1);

	ID = _ID;

	if (ID == 1)
		textureFilePath = "Content/tallgrass.png";
	if (ID == 2)
		textureFilePath = "Content/torch.png";
	if (ID == 3) {
		textureFilePath = "Content/guy.png";
		Scale = vec3(2, 2, 2);
	}
	if (ID == 4) {
		textureFilePath = "Content/TorchSheet.png";
		AnimTimer->SourceFrames = 7;
		Scale = vec3(2, 2, 2);
		SelfIlluminated = 1;
		PointLight * P = new PointLight(Position, vec3(238, 181, 128) / 255.0f, 06, 1.1f);
		Main::PointLights.push_back(P);
	}
	
	Origin = vec3(-.5f, 0, 0) * Scale;
}

Billboard::~Billboard()
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
}

void Billboard::Init() {
	if (!isInit) {
		isInit = true;

		glGenTextures(1, &textureID);
		glBindTexture(GL_TEXTURE_2D, textureID);

		MainTexture = Texture2D(textureFilePath);

		//float t_GridTexture[16] = {
		//	1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 1.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
		//};

		//Define texture images
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, t_GridTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, MainTexture.width, MainTexture.height, 0, GL_RGBA, GL_FLOAT, MainTexture.Pixels);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the filter parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);


		glGenTextures(1, &textureID2);
		glBindTexture(GL_TEXTURE_2D, textureID2);

		Texture2D texture2 = Texture2D(textureFilePath2);

		//float t_GridTexture[16] = {
		//	1.0f, 0.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f, 1.0f,
		//	0.0f, 0.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f, 1.0f,
		//};

		//Define texture images
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 2, 2, 0, GL_RGBA, GL_FLOAT, t_GridTexture);

		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture2.width, texture2.height, 0, GL_RGBA, GL_FLOAT, texture2.Pixels);

		glGenerateMipmap(GL_TEXTURE_2D);

		// Set the filter parameters
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);
		//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		curTexID = textureID;

		shader = ShaderPipeline("Shaders/StandardVert.vert", "Shaders/StandardFrag.frag");
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
	glBindTexture(GL_TEXTURE_2D, curTexID);

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
	glBindTexture(GL_TEXTURE_2D, curTexID);
	glUniform1i(glGetUniformLocation(shaderProgram, "MainTexture"), 0);

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

	glUniform1i(glGetUniformLocation(shaderProgram, "SourceFrames"), AnimTimer->SourceFrames);

	glUniform1i(glGetUniformLocation(shaderProgram, "CurrentFrame"), AnimTimer->CurrentFrame);

	glUniform1i(glGetUniformLocation(shaderProgram, "SelfIlluminated"), SelfIlluminated);

	glUniform1i(glGetUniformLocation(shaderProgram, "Outlined"), Outlined);

	glUniform2f(glGetUniformLocation(shaderProgram, "TextureSize"), 
		MainTexture.width, MainTexture.height);

	for (int i = 0; i < Main::PointLights.size(); i++) {
		string _i = std::to_string(i);

		glUniform3f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Position").c_str()),
			Main::PointLights[i]->Position.x, Main::PointLights[i]->Position.y, Main::PointLights[i]->Position.z);

		glUniform3f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Color").c_str()),
			Main::PointLights[i]->Color.x, Main::PointLights[i]->Color.y, Main::PointLights[i]->Color.z);

		glUniform1f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Range").c_str()),
			Main::PointLights[i]->Range);

		glUniform1f(glGetUniformLocation(shaderProgram, ("PointLights[" + _i + "].Brightness").c_str()),
			Main::PointLights[i]->Brightness);
	}
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
	
	if (Main::TapKeys[GLFW_KEY_I]) {
		Outlined *= -1;
	}

	Init();
	AnimTimer->Update();

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
