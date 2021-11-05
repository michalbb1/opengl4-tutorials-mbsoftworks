// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "006-camera-pt2-flying-camera.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"
#include "../common_classes/staticGeometry.h"
#include "../common_classes/flyingCamera.h"

Shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
VertexBufferObject shapesVBO;
VertexBufferObject colorsVBO;

GLuint mainVAO;

FlyingCamera camera(glm::vec3(0.0f, 8.0f, 20.0f), glm::vec3(0.0f, 8.0f, 19.0f), glm::vec3(0.0f, 1.0f, 0.0f));
float rotationAngleRad; // in radians

void OpenGLWindow006::initializeScene()
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
	shapesVBO.addData(static_geometry::plainGroundVertices);
	shapesVBO.addData(static_geometry::cubeVertices);
	shapesVBO.addData(static_geometry::pyramidVertices);
	shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void*>(0));

	// Setup colors now
	colorsVBO.createVBO();
	colorsVBO.bindVBO();

	colorsVBO.addData(static_geometry::plainGroundColors);
	colorsVBO.addData(static_geometry::cubeFaceColors, 6);
	colorsVBO.addData(static_geometry::pyramidFaceColors, 4);

	colorsVBO.uploadDataToGPU(GL_STATIC_DRAW);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void*>(0));

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void OpenGLWindow006::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram.useProgram();
	glBindVertexArray(mainVAO);

	mainProgram["matrices.projectionMatrix"] = getProjectionMatrix();
	mainProgram["matrices.viewMatrix"] = camera.getViewMatrix();

	// Render ground
	mainProgram["matrices.modelMatrix"] = glm::mat4(1.0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Render "houses" on the left
	for (auto i = 0; i < 10; i++)
	{
		// Lets' predefine some sizes
		auto houseBottomSize = 10.0f;
		auto roofTopSize = 12.0f;

		// First, calculate the basic position of house
		auto modelMatrixHouse = glm::mat4(1.0);
		modelMatrixHouse = glm::translate(modelMatrixHouse, glm::vec3(-40.0f, 0.0f, -125.0f + i * 25.0f));

		// Render bottom cube of the house
		glm::mat4 modelMatrixBottom = glm::translate(modelMatrixHouse, glm::vec3(0.0f, houseBottomSize / 2.0f, 0.0f));
		modelMatrixBottom = glm::rotate(modelMatrixBottom, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixBottom = glm::scale(modelMatrixBottom, glm::vec3(houseBottomSize, houseBottomSize, houseBottomSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixBottom;
		glDrawArrays(GL_TRIANGLES, 4, 36);

		// Render top (roof) of the house
		auto translateTopY = houseBottomSize + roofTopSize / 2.0f - 1.0f;
		glm::mat4 modelMatrixTop = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateTopY, 0.0f));
		modelMatrixTop = glm::rotate(modelMatrixTop, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixTop = glm::scale(modelMatrixTop, glm::vec3(roofTopSize, roofTopSize, roofTopSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixTop;
		glDrawArrays(GL_TRIANGLES, 40, 12);
	}

	// Render "skyscrapers" on the right
	for (auto i = 0; i < 10; i++)
	{
		auto houseBottomSize = 10.0f;
		auto houseMiddleSize = 7.0f;
		auto houseTopSize = 4.0f;

		// First, calculate the basic position of skyscraper
		auto modelMatrixHouse = glm::mat4(1.0);
		modelMatrixHouse = glm::translate(modelMatrixHouse, glm::vec3(40.0f, 0.0f, -125.0f + i * 25.0f));

		// Render the bottom part of skyscraper
		glm::mat4 modelMatrixBottom = glm::translate(modelMatrixHouse, glm::vec3(0.0f, houseBottomSize / 2.0f, 0.0f));
		modelMatrixBottom = glm::rotate(modelMatrixBottom, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixBottom = glm::scale(modelMatrixBottom, glm::vec3(houseBottomSize, houseBottomSize, houseBottomSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixBottom;
		glDrawArrays(GL_TRIANGLES, 4, 36);

		// Render the middle part of skyscraper
		auto translateMiddleY = houseBottomSize + houseMiddleSize / 2.0f;
		glm::mat4 modelMatrixMiddle = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateMiddleY, 0.0f));
		modelMatrixMiddle = glm::rotate(modelMatrixMiddle, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixMiddle = glm::scale(modelMatrixMiddle, glm::vec3(houseMiddleSize, houseMiddleSize, houseMiddleSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixMiddle;
		glDrawArrays(GL_TRIANGLES, 4, 36);

		// Render the top part of skyscraper
		auto translateTopY = houseMiddleSize + houseBottomSize + houseTopSize / 2.0f;
		glm::mat4 modelMatrixTop = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateTopY, 0.0f));
		modelMatrixTop = glm::rotate(modelMatrixTop, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixTop = glm::scale(modelMatrixTop, glm::vec3(houseTopSize, houseTopSize, houseTopSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixTop;
		glDrawArrays(GL_TRIANGLES, 4, 36);
	}
}

void OpenGLWindow006::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    int posX, posY, width, height;
    glfwGetWindowPos(getWindow(), &posX, &posY);
    glfwGetWindowSize(getWindow(), &width, &height);
    camera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));

    camera.update([this](int keyCode) {return this->keyPressed(keyCode); },
        [this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
        [this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
        [this](float f) {return this->sof(f); });

    rotationAngleRad += glm::radians(sof(45.0f));

    std::string windowTitleWithFPS = "006.) Camera pt. 2 - Flying Camera - Tutorial by Michal Bubnar (www.mbsoftworks.sk) - FPS: "
        + std::to_string(getFPS()) +
        ", VSync: " + (isVerticalSynchronizationEnabled() ? "On" : "Off") + " (Press F3 to toggle)";
    glfwSetWindowTitle(getWindow(), windowTitleWithFPS.c_str());
}

void OpenGLWindow006::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();
	colorsVBO.deleteVBO();

	glDeleteVertexArrays(1, &mainVAO);
}
