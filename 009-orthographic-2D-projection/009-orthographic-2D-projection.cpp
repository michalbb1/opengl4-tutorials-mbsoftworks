// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "009-orthographic-2D-projection.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"
#include "../common_classes/staticGeometry.h"
#include "../common_classes/flyingCamera.h"
#include "../common_classes/texture.h"
#include "../common_classes/sampler.h"

Shader groundVertexShader, groundFragmentShader;
Shader vertexShader, fragmentShader;
Shader ortho2DVertexShader, ortho2DFragmentShader;

ShaderProgram groundProgram;
ShaderProgram mainProgram;
ShaderProgram ortho2DProgram;

VertexBufferObject shapesVBO;
VertexBufferObject texCoordsVBO;
GLuint mainVAO;

VertexBufferObject hudVerticesVBO;
VertexBufferObject hudTexCoordsVBO;
GLuint hudVAO;

FlyingCamera camera(glm::vec3(-120.0f, 8.0f, 120.0f), glm::vec3(-120.0f, 8.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);

Texture snowTexture;
Texture pathTexture;
Texture pavementTexture;
Texture houseTexture;
Texture houseTextureFront;
Texture houseTextureSide;
Texture roofTexture;
Texture christmasTree;
Texture snowflake;

Sampler mainSampler;
Sampler hudSampler;

bool blendingEnabled = true;

struct HouseTransformation
{
	glm::vec3 position;
	float angle;
};

std::vector<HouseTransformation> houseTransformations
{
	{glm::vec3(-140, 0, 94), glm::radians(90.0f)},
	{glm::vec3(-140, 0, 64), glm::radians(90.0f)},
	{glm::vec3(-140, 0, 34), glm::radians(90.0f)},
	{glm::vec3(-140, 0, 4), glm::radians(90.0f)},
	{glm::vec3(-97, 0, 18), glm::radians(-90.0f)},
	{glm::vec3(-97, 0, 48), glm::radians(-90.0f)},
	{glm::vec3(-97, 0, 78), glm::radians(-90.0f)},
	{glm::vec3(-63, 0, -14), glm::radians(180.0f)},
	{glm::vec3(-33, 0, -14), glm::radians(180.0f)},
	{glm::vec3(-3, 0, -14), glm::radians(180.0f)},
	{glm::vec3(27, 0, -14), glm::radians(180.0f)},
	{glm::vec3(-75, 0, -64), glm::radians(0.0f)},
	{glm::vec3(-45, 0, -64), glm::radians(0.0f)},
	{glm::vec3(-15, 0, -64), glm::radians(0.0f)},
	{glm::vec3(15, 0, -64), glm::radians(0.0f)},
	{glm::vec3(45, 0, -64), glm::radians(0.0f)},
	{glm::vec3(-121, 0, -54), glm::radians(45.0f)}
};

void OpenGLWindow009::initializeScene()
{
	glClearColor(0.0f, 0.28f, 0.57f, 1.0f);

	vertexShader.loadShaderFromFile("data/shaders/tut007/shader.vert", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("data/shaders/tut007/shader.frag", GL_FRAGMENT_SHADER);

	groundVertexShader.loadShaderFromFile("data/shaders/tut008/ground_shader.vert", GL_VERTEX_SHADER);
	groundFragmentShader.loadShaderFromFile("data/shaders/tut008/ground_shader.frag", GL_FRAGMENT_SHADER);

	ortho2DVertexShader.loadShaderFromFile("data/shaders/tut009/ortho2D.vert", GL_VERTEX_SHADER);
	ortho2DFragmentShader.loadShaderFromFile("data/shaders/tut009/ortho2D.frag", GL_FRAGMENT_SHADER);

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

	groundProgram.createProgram();
	groundProgram.addShaderToProgram(groundVertexShader);
	groundProgram.addShaderToProgram(groundFragmentShader);

	if (!groundProgram.linkProgram())
	{
		closeWindow(true);
		return;
	}

	ortho2DProgram.createProgram();
	ortho2DProgram.addShaderToProgram(ortho2DVertexShader);
	ortho2DProgram.addShaderToProgram(ortho2DFragmentShader);

	if (!ortho2DProgram.linkProgram())
	{
		closeWindow(true);
		return;
	}

	glGenVertexArrays(1, &mainVAO); // Creates one Vertex Array Object
	glBindVertexArray(mainVAO);

	// Setup vertex positions first
	shapesVBO.createVBO();
	shapesVBO.bindVBO();
	shapesVBO.addRawData(static_geometry::plainGroundVertices, sizeof(static_geometry::plainGroundVertices));
	shapesVBO.addRawData(static_geometry::cubeVertices, sizeof(static_geometry::cubeVertices));
	shapesVBO.addRawData(static_geometry::pyramidVertices, sizeof(static_geometry::pyramidVertices));
	shapesVBO.uploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), static_cast<void*>(0));

	// Setup texture coordinates next
	texCoordsVBO.createVBO();
	texCoordsVBO.bindVBO();
	texCoordsVBO.addRawData(static_geometry::plainGroundTexCoords, sizeof(static_geometry::plainGroundTexCoords));
	texCoordsVBO.addRawData(static_geometry::cubeTexCoords, sizeof(static_geometry::cubeTexCoords), 6);
	texCoordsVBO.addRawData(static_geometry::pyramidTexCoords, sizeof(static_geometry::pyramidTexCoords), 4);
	
	texCoordsVBO.uploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), static_cast<void*>(0));

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);

	snowTexture.loadTexture2D("data/textures/snow.png");
	pathTexture.loadTexture2D("data/textures/tut008/path.png");
	houseTexture.loadTexture2D("data/textures/brick.png");
	houseTextureFront.loadTexture2D("data/textures/tut008/house_texture_front.png");
	houseTextureSide.loadTexture2D("data/textures/tut008/house_texture_side.png");
	roofTexture.loadTexture2D("data/textures/prismarine_dark.png");
	pavementTexture.loadTexture2D("data/textures/pavement.jpg");
	christmasTree.loadTexture2D("data/textures/tut009/christmas_tree.png", false);
	snowflake.loadTexture2D("data/textures/tut009/snowflake.png", false);

	mainSampler.create();
	mainSampler.bind();
	mainSampler.setMagnificationFilter(MAG_FILTER_BILINEAR);
	mainSampler.setMinificationFilter(MIN_FILTER_TRILINEAR);

	hudSampler.create();
	hudSampler.bind();
	hudSampler.setMagnificationFilter(MAG_FILTER_BILINEAR);
	hudSampler.setMinificationFilter(MIN_FILTER_BILINEAR);

	glGenVertexArrays(1, &hudVAO); // Creates one Vertex Array Object
	glBindVertexArray(hudVAO);

	hudVerticesVBO.createVBO();
	hudVerticesVBO.bindVBO();
	hudVerticesVBO.addData(static_geometry::quad2D, sizeof(glm::vec2) * 4);

	hudVerticesVBO.uploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), static_cast<void*>(0));

	hudTexCoordsVBO.createVBO();
	hudTexCoordsVBO.bindVBO();
	hudTexCoordsVBO.addData(static_geometry::quad2D, sizeof(glm::vec2) * 4);

	hudTexCoordsVBO.uploadDataToGPU(GL_STATIC_DRAW);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), static_cast<void*>(0));
}

void OpenGLWindow009::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	groundProgram.useProgram();
	glBindVertexArray(mainVAO);

	groundProgram["matrices.projectionMatrix"] = getProjectionMatrix();
	groundProgram["matrices.viewMatrix"] = camera.getViewMatrix();

	// Render ground
	groundProgram["matrices.modelMatrix"] = glm::mat4(1.0);

	// Setup snow texture
	snowTexture.bind(0);
	mainSampler.bind(0);
	groundProgram["snowSampler"] = 0;

	// Setup path texture
	pathTexture.bind(1);
	mainSampler.bind(1);
	groundProgram["pathSampler"] = 1;

	// Setup pavement texture
	pavementTexture.bind(2);
	mainSampler.bind(2);
	groundProgram["pavementSampler"] = 2;

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	mainProgram.useProgram();
	mainProgram["matrices.projectionMatrix"] = getProjectionMatrix();
	mainProgram["matrices.viewMatrix"] = camera.getViewMatrix();
	mainProgram["color"] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	const auto houseBottomSize = 10.0f;
	const auto roofTopSize = 12.0f;
	
	// Render "houses" on the left
	for (auto& houseTransformation : houseTransformations)
	{
		const auto housePosition = houseTransformation.position;
		const auto angle = houseTransformation.angle;
		// First, calculate the basic position of house
		auto modelMatrixHouse = glm::mat4(1.0);
		modelMatrixHouse = glm::translate(modelMatrixHouse, housePosition);

		// Render bottom cube of the house
		glm::mat4 modelMatrixBottom = glm::translate(modelMatrixHouse, glm::vec3(0.0f, houseBottomSize / 2.0f, 0.0f));
		modelMatrixBottom = glm::rotate(modelMatrixBottom, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixBottom = glm::scale(modelMatrixBottom, glm::vec3(houseBottomSize, houseBottomSize, houseBottomSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixBottom;

		// Render just plain house walls made of bricks
		houseTexture.bind();
		glDrawArrays(GL_TRIANGLES, 10, 6);
		glDrawArrays(GL_TRIANGLES, 28, 6);
		// Render house wall with a door and windows
		houseTextureFront.bind();
		glDrawArrays(GL_TRIANGLES, 4, 6);
		// Render house wall with a window
		houseTextureSide.bind();
		glDrawArrays(GL_TRIANGLES, 16, 12);

		roofTexture.bind();
		const auto translateTopY = houseBottomSize + roofTopSize / 2.0f - 0.25f;
		glm::mat4 modelMatrixTop = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateTopY, 0.0f));
		modelMatrixTop = glm::rotate(modelMatrixTop, angle, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixTop = glm::scale(modelMatrixTop, glm::vec3(roofTopSize, roofTopSize, roofTopSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixTop;
		glDrawArrays(GL_TRIANGLES, 40, 12);
	}

	glDisable(GL_DEPTH_TEST);

	if (blendingEnabled)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}
	glDepthMask(0);

	int width, height;
	glfwGetWindowSize(getWindow(), &width, &height);
	
	ortho2DProgram.useProgram();
	glBindVertexArray(hudVAO);
	hudSampler.bind();
	ortho2DProgram["matrices.projectionMatrix"] = getOrthoProjectionMatrix();
	ortho2DProgram["sampler"] = 0;
	ortho2DProgram["color"] = glm::vec4(1.0, 1.0, 1.0, 1.0);

	// Render Christmas tree bottom left
	glm::mat4 model = glm::mat4(1.0);
	model = glm::scale(model, glm::vec3(christmasTree.getWidth(), christmasTree.getHeight(), 1));
	ortho2DProgram["matrices.modelMatrix"] = model;
	christmasTree.bind(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	// Render snowflake bottom right
	model = glm::translate(glm::mat4(1.0), glm::vec3(width - christmasTree.getWidth(), 0, 0));
	model = glm::scale(model, glm::vec3(snowflake.getWidth(), snowflake.getHeight(), 1));
	ortho2DProgram["matrices.modelMatrix"] = model;
	snowflake.bind(0);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	
	if (blendingEnabled) {
		glDisable(GL_BLEND);
	}
	glEnable(GL_DEPTH_TEST);
	glDepthMask(1);
}

void OpenGLWindow009::updateScene()
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

    if (keyPressedOnce(GLFW_KEY_F4)) {
        blendingEnabled = !blendingEnabled;
    }

    std::string windowTitleWithFPS = std::string("009.) Orthographic 2D Projection - Tutorial by Michal Bubnar (www.mbsoftworks.sk) - ")
        + "FPS: " + std::to_string(getFPS())
        + ", VSync: " + (isVerticalSynchronizationEnabled() ? "On" : "Off") + " (Press F3 to toggle)"
        + ", Blending: " + (blendingEnabled ? "On" : "Off") + " (Press F4 to toggle)";

    glfwSetWindowTitle(getWindow(), windowTitleWithFPS.c_str());
}


void OpenGLWindow009::releaseScene()
{
	groundProgram.deleteProgram();
	mainProgram.deleteProgram();

	groundVertexShader.deleteShader();
	groundFragmentShader.deleteShader();
	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();
	texCoordsVBO.deleteVBO();
	hudVerticesVBO.deleteVBO();
	hudTexCoordsVBO.deleteVBO();

	snowTexture.deleteTexture();
	pathTexture.deleteTexture();
	pavementTexture.deleteTexture();
	houseTexture.deleteTexture();
	houseTextureFront.deleteTexture();
	houseTextureSide.deleteTexture();
	roofTexture.deleteTexture();
	christmasTree.deleteTexture();
	snowflake.deleteTexture();

	mainSampler.deleteSampler();

	glDeleteVertexArrays(1, &mainVAO);
	glDeleteVertexArrays(1, &hudVAO);
}
