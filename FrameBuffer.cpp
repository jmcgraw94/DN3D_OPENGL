#include "FrameBuffer.h"
#include "Main.h"

FrameBuffer::FrameBuffer()
{
}

FrameBuffer::~FrameBuffer()
{
}

//http://www.mathopenref.com/index.html

float rot = 0;

GLuint FrameBuffer::ReserveScreenRectTexture()
{
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, WINW, WINH, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	return textureID;
}

void FrameBuffer::Dispose() {
	//Release memory
	glDeleteFramebuffers(1, &FrameBufferObject);
}

void FrameBuffer::BindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
}

void FrameBuffer::CleanseBuffer() {
	//Clear any unnecessary buffer information
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::DrawFrameBuffer() {
	//Switch to the Screen Shader
	ScreenShader.Use();
	glUseProgram(ScreenShader.Program);
	glBindVertexArray(quadVAO);

	mat4 model = mat4();

	if (Main::FrameFuck) {
		rot += Main::DeltaTime * 10;

		model = glm::rotate(model, glm::radians(0.f), vec3(0, 0, 1));
		model = glm::rotate(model, glm::radians(rot), vec3(0, 1, 0));
		model = glm::rotate(model, glm::radians(rot), vec3(1, 0, 0));

		model = glm::scale(model, glm::vec3(1, 1, 1));
	}
	else
		rot = 0;
	//Uniforms
	glUniform1i(glGetUniformLocation(ScreenShader.Program, "BitDepth"), Main::ColorBitDepth);

	glUniformMatrix4fv(glGetUniformLocation(ScreenShader.Program, "model"),
		1, GL_FALSE, glm::value_ptr(model));

	//Activate the current 
	glBindTexture(GL_TEXTURE_2D, TextureColorBuffer);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

//Perform any setup necessary for each buffer
void FrameBuffer::Setup() {

	GLchar * ScreenVertexShaderPath = "Shaders/ScreenVert.vert";
	GLchar * ScreenFragmentShaderPath = "Shaders/ScreenFrag.frag";
	ScreenShader = ShaderPipeline(ScreenVertexShaderPath, ScreenFragmentShaderPath);

	// Generate Arrays and Buffers
	glGenFramebuffers(1, &FrameBufferObject);
	glGenRenderbuffers(1, &RenderBufferObject);
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	//Reserve space for a screen rect buffer
	TextureColorBuffer = ReserveScreenRectTexture();
	//Activate the Framebuffer for following operations
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
	//Attach the recently reserved color buffer 
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureColorBuffer, 0);

	//Render Buffer Object Setup
	glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferObject);
	//Place into storage
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINW, WINH);
	//Unbind unneeded Render Buffer Object
	glBindRenderbuffer(GL_RENDERBUFFER, 0);

	//Provide Render Buffer Object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferObject); 
	//Check for completion
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR: Incomplete FrameBuffer" << endl;

	//Finally unbind the framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//Bind the Quad's Vertex Array Object
	glBindVertexArray(quadVAO);
	//Bind the Vertex Buffer Object
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
	//Buffer object
	glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenVertices), &ScreenVertices, GL_STATIC_DRAW);
	//Vec2 Stride
	int Stride = 2;
	//Enable Position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);
	//Enable TextCoords 
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(Stride * sizeof(GLfloat)));
	//Unbind active Vertex Array Object
	glBindVertexArray(0);
}