// Project
#include "003-adding-colors.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"

Shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
VertexBufferObject shapesVBO;
VertexBufferObject colorsVBO;

GLuint mainVAO;

void OpenGLWindow003::initializeScene()
{
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	vertexShader.loadShaderFromFile("data/shaders/tut003/shader.vert", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("data/shaders/tut003/shader.frag", GL_FRAGMENT_SHADER);

	if (!vertexShader.isLoaded() || !fragmentShader.isLoaded())
	{
		closeWindow(true);
		return;
	}

	mainProgram.createProgram();
	mainProgram.addShaderToProgram(vertexShader);
	mainProgram.addShaderToProgram(fragmentShader);

	if (!mainProgram.linkProgram())
	{
		closeWindow(true);
		return;
	}

	glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
	glBindVertexArray(mainVAO);

	glm::vec3 vTriangle[] = { glm::vec3(-0.4f, 0.1f, 0.0f), glm::vec3(0.4f, 0.1f, 0.0f), glm::vec3(0.0f, 0.7f, 0.0f) };
	glm::vec3 vTriangleColors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f),glm::vec3(0.0f, 0.0f, 1.0f) };

	glm::vec3 vQuad[] = { glm::vec3(-0.2f, -0.1f, 0.0f), glm::vec3(-0.2f, -0.6f, 0.0f), glm::vec3(0.2f, -0.1f, 0.0f), glm::vec3(0.2f, -0.6f, 0.0f) };
	glm::vec3 vQuadColors[] = { glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(1.0f, 0.5f, 0.0f) };
	
	shapesVBO.createVBO();
	shapesVBO.bindVBO();
	shapesVBO.addData(vTriangle);
	shapesVBO.addData(vQuad);
	shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);

	colorsVBO.createVBO();
	colorsVBO.bindVBO();
	colorsVBO.addData(vTriangleColors);
	colorsVBO.addData(vQuadColors);
	colorsVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), 0);
}

void OpenGLWindow003::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram.useProgram();
	glBindVertexArray(mainVAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
}

void OpenGLWindow003::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE))
        closeWindow();
}

void OpenGLWindow003::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();
	colorsVBO.deleteVBO();

	glDeleteVertexArrays(1, &mainVAO);
}