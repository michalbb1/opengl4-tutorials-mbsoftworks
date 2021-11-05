// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "004-entering-third-dimension.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"
#include "../common_classes/staticGeometry.h"

Shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
VertexBufferObject shapesVBO;
VertexBufferObject colorsVBO;

GLuint mainVAO;

float rotationAngleRad;

void OpenGLWindow004::initializeScene()
{
	glClearColor(0.0f, 0.28f, 0.57f, 1.0f);

	vertexShader.loadShaderFromFile("data/shaders/tut004/shader.vert", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("data/shaders/tut004/shader.frag", GL_FRAGMENT_SHADER);

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

	// Setup vertex positions first
	shapesVBO.createVBO();
	shapesVBO.bindVBO();
	shapesVBO.addData(static_geometry::cubeVertices);
	shapesVBO.addData(static_geometry::pyramidVertices);
	shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void*>(0));

	// Setup colors now
	colorsVBO.createVBO();
	colorsVBO.bindVBO();
	colorsVBO.addData(static_geometry::cubeFaceColors, 6);
	colorsVBO.addData(static_geometry::pyramidFaceColors, 6);

	colorsVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void*>(0));

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void OpenGLWindow004::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram.useProgram();
	glBindVertexArray(mainVAO);

	mainProgram["matrices.projectionMatrix"] = getProjectionMatrix();
	mainProgram["matrices.viewMatrix"] = glm::lookAt(glm::vec3(0.0f, 0.0f, 20.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	// Render rotating cube in the middle
	auto modelMatrixCube = glm::mat4(1.0);
	modelMatrixCube = glm::rotate(modelMatrixCube, rotationAngleRad, glm::vec3(1.0f, 0.0f, 0.0f));
	modelMatrixCube = glm::rotate(modelMatrixCube, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
	modelMatrixCube = glm::rotate(modelMatrixCube, rotationAngleRad, glm::vec3(0.0f, 0.0f, 1.0f));
	modelMatrixCube = glm::scale(modelMatrixCube, glm::vec3(5.0f, 5.0f, 5.0f));
	
	mainProgram["matrices.modelMatrix"] = modelMatrixCube;
	glDrawArrays(GL_TRIANGLES, 0, 36);

	// Render 4 pyramids around the cube with the positions defined in the following array
	glm::vec3 pyramidTranslationVectors[] =
	{
		glm::vec3(-12.0f, 7.0f, 0.0f),
		glm::vec3(12.0f, 7.0f, 0.0f),
		glm::vec3(12.0f, -7.0f, 0.0f),
		glm::vec3(-12.0f, -7.0f, 0.0f)
	};

	for (auto pyramidTranslation : pyramidTranslationVectors)
	{
		glm::mat4 modelMatrixPyramid = glm::mat4(1.0);
		modelMatrixPyramid = glm::translate(modelMatrixPyramid, pyramidTranslation);
		modelMatrixPyramid = glm::rotate(modelMatrixPyramid, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixPyramid = glm::scale(modelMatrixPyramid, glm::vec3(3.0f, 3.0f, 3.0f));

		mainProgram["matrices.modelMatrix"] = modelMatrixPyramid;
		glDrawArrays(GL_TRIANGLES, 36, 12); // Pyramid vertices start after cube, that is on index 36
	}
}

void OpenGLWindow004::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }

    rotationAngleRad += glm::radians(sof(90.0f));

    std::string windowTitleWithFPS = "004.) Entering Third Dimension - Tutorial by Michal Bubnar (www.mbsoftworks.sk) - FPS: " + std::to_string(getFPS());
    glfwSetWindowTitle(getWindow(), windowTitleWithFPS.c_str());
}

void OpenGLWindow004::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();
	colorsVBO.deleteVBO();

	glDeleteVertexArrays(1, &mainVAO);
}
