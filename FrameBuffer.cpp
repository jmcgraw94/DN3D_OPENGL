#include "FrameBuffer.h"
#include "Main.h"


FrameBuffer::FrameBuffer()
{
}


FrameBuffer::~FrameBuffer()
{
}

GLuint RenderBufferObject;
GLuint FrameBufferObject;
GLuint TextureColorbuffer;
ShaderPipeline ScreenShader;
GLuint quadVAO, quadVBO;
GLuint texture;

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
	glDeleteFramebuffers(1, &FrameBufferObject);
}

void FrameBuffer::BindFrameBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
}

void FrameBuffer::CleanseBuffer() {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClearColor(0, 0, 0, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);
}

void FrameBuffer::DrawFrameBuffer() {
	ScreenShader.Use();
	glUseProgram(ScreenShader.Program);
	glBindVertexArray(quadVAO);
	glBindTexture(GL_TEXTURE_2D, TextureColorbuffer);	// Use the color attachment texture as the texture of the quad plane
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindVertexArray(0);
}

void FrameBuffer::Setup() {
	GLchar * ScreenVertexShaderPath = "Shaders/ScreenVert.vert";
	GLchar * ScreenFragmentShaderPath = "Shaders/ScreenFrag.frag";
	ScreenShader = ShaderPipeline(ScreenVertexShaderPath, ScreenFragmentShaderPath);


	// Generate Arrays and Buffers
	glGenFramebuffers(1, &FrameBufferObject);
	glGenRenderbuffers(1, &RenderBufferObject);
	glGenVertexArrays(1, &quadVAO);
	glGenBuffers(1, &quadVBO);

	TextureColorbuffer = ReserveScreenRectTexture();
	glBindFramebuffer(GL_FRAMEBUFFER, FrameBufferObject);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, TextureColorbuffer, 0);
	// Create a renderbuffer object for depth and stencil attachment (we won't be sampling these)

	glBindRenderbuffer(GL_RENDERBUFFER, RenderBufferObject);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, WINW, WINH); // Use a single renderbuffer object for both a depth AND stencil buffer.
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, RenderBufferObject); // Now actually attach it
																												 // Now that we actually created the framebuffer and added all attachments we want to check if it is actually complete now
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << endl;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

	glBufferData(GL_ARRAY_BUFFER, sizeof(ScreenVertices), &ScreenVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (GLvoid*)(2 * sizeof(GLfloat)));

	glBindVertexArray(0);
}