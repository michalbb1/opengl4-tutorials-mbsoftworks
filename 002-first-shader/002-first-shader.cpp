// Project
#include "002-first-shader.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"

namespace opengl4_mbsoftworks {
namespace tutorial002 {

Shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
VertexBufferObject shapesVBO;

GLuint mainVAO;

void OpenGLWindow002::initializeScene()
{
	glClearColor(0.0f, 0.5f, 1.0f, 1.0f);

	vertexShader.loadShaderFromFile("data/shaders/tut002/shader.vert", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("data/shaders/tut002/shader.frag", GL_FRAGMENT_SHADER);

	if (!vertexShader.isCompiled() || !fragmentShader.isCompiled())
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

	constexpr glm::vec3 triangleVertices[] = { glm::vec3(-0.4f, 0.1f, 0.0f), glm::vec3(0.4f, 0.1f, 0.0f), glm::vec3(0.0f, 0.7f, 0.0f) };
	constexpr glm::vec3 quadVertices[] = { glm::vec3(-0.2f, -0.1f, 0.0f), glm::vec3(-0.2f, -0.6f, 0.0f), glm::vec3(0.2f, -0.1f, 0.0f), glm::vec3(0.2f, -0.6f, 0.0f) };

	shapesVBO.createVBO();
	shapesVBO.bindVBO();
	shapesVBO.addData(triangleVertices);
	shapesVBO.addData(quadVertices);
	shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), nullptr);
}

void OpenGLWindow002::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram.useProgram();
	glBindVertexArray(mainVAO);

	glDrawArrays(GL_TRIANGLES, 0, 3);
	glDrawArrays(GL_TRIANGLE_STRIP, 3, 4);
}

void OpenGLWindow002::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }
}

void OpenGLWindow002::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();

	glDeleteVertexArrays(1, &mainVAO);
}

} // namespace tutorial002
} // namespace opengl4_mbsoftworks