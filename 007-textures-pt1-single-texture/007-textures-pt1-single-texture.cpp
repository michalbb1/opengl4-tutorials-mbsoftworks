// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "007-textures-pt1-single-texture.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"
#include "../common_classes/staticGeometry.h"
#include "../common_classes/flyingCamera.h"
#include "../common_classes/texture.h"
#include "../common_classes/sampler.h"

Shader vertexShader, fragmentShader;
ShaderProgram mainProgram;
VertexBufferObject shapesVBO;
VertexBufferObject texCoordsVBO;

GLuint mainVAO;

FlyingCamera camera(glm::vec3(0.0f, 8.0f, 20.0f), glm::vec3(0.0f, 8.0f, 19.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);

Texture groundTexture;
Texture houseTexture;
Texture roofTexture;

Texture filterTextures[10];
Sampler samplersShowcase[10];
int groundSamplerIndex = 9;

void OpenGLWindow007::initializeScene()
{
	glClearColor(0.0f, 0.28f, 0.57f, 1.0f);

	vertexShader.loadShaderFromFile("data/shaders/tut007/shader.vert", GL_VERTEX_SHADER);
	fragmentShader.loadShaderFromFile("data/shaders/tut007/shader.frag", GL_FRAGMENT_SHADER);

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

	groundTexture.loadTexture2D("data/textures/clay.png");
	houseTexture.loadTexture2D("data/textures/brick.png");
	roofTexture.loadTexture2D("data/textures/prismarine_dark.png");

	filterTextures[0].loadTexture2D("data/textures/tut007/mag_nearest_min_nearest.png");
	filterTextures[1].loadTexture2D("data/textures/tut007/mag_nearest_min_bilinear.png");
	filterTextures[2].loadTexture2D("data/textures/tut007/mag_nearest_min_nearest_mipmap.png");
	filterTextures[3].loadTexture2D("data/textures/tut007/mag_nearest_min_linear_mipmap.png");
	filterTextures[4].loadTexture2D("data/textures/tut007/mag_nearest_min_linear_mipmap_linear.png");

	filterTextures[5].loadTexture2D("data/textures/tut007/mag_linear_min_nearest.png");
	filterTextures[6].loadTexture2D("data/textures/tut007/mag_linear_min_linear.png");
	filterTextures[7].loadTexture2D("data/textures/tut007/mag_linear_min_nearest_mipmap.png");
	filterTextures[8].loadTexture2D("data/textures/tut007/mag_linear_min_linear_mipmap.png");
	filterTextures[9].loadTexture2D("data/textures/tut007/mag_linear_min_linear_mipmap_linear.png");

	for (auto& sampler : samplersShowcase) {
		sampler.create();
	}

	samplersShowcase[0].bind();
	samplersShowcase[0].setMagnificationFilter(MAG_FILTER_NEAREST);
	samplersShowcase[0].setMinificationFilter(MIN_FILTER_NEAREST);

	samplersShowcase[1].bind();
	samplersShowcase[1].setMagnificationFilter(MAG_FILTER_NEAREST);
	samplersShowcase[1].setMinificationFilter(MIN_FILTER_BILINEAR);

	samplersShowcase[2].bind();
	samplersShowcase[2].setMagnificationFilter(MAG_FILTER_NEAREST);
	samplersShowcase[2].setMinificationFilter(MIN_FILTER_NEAREST_MIPMAP);

	samplersShowcase[3].bind();
	samplersShowcase[3].setMagnificationFilter(MAG_FILTER_NEAREST);
	samplersShowcase[3].setMinificationFilter(MIN_FILTER_BILINEAR);

	samplersShowcase[4].bind();
	samplersShowcase[4].setMagnificationFilter(MAG_FILTER_NEAREST);
	samplersShowcase[4].setMinificationFilter(MIN_FILTER_TRILINEAR);

	samplersShowcase[5].bind();
	samplersShowcase[5].setMagnificationFilter(MAG_FILTER_BILINEAR);
	samplersShowcase[5].setMinificationFilter(MIN_FILTER_NEAREST);

	samplersShowcase[6].bind();
	samplersShowcase[6].setMagnificationFilter(MAG_FILTER_BILINEAR);
	samplersShowcase[6].setMinificationFilter(MIN_FILTER_BILINEAR);

	samplersShowcase[7].bind();
	samplersShowcase[7].setMagnificationFilter(MAG_FILTER_BILINEAR);
	samplersShowcase[7].setMinificationFilter(MIN_FILTER_NEAREST_MIPMAP);

	samplersShowcase[8].bind();
	samplersShowcase[8].setMagnificationFilter(MAG_FILTER_BILINEAR);
	samplersShowcase[8].setMinificationFilter(MIN_FILTER_BILINEAR);

	samplersShowcase[9].bind();
	samplersShowcase[9].setMagnificationFilter(MAG_FILTER_BILINEAR);
	samplersShowcase[9].setMinificationFilter(MIN_FILTER_TRILINEAR);
}

void OpenGLWindow007::renderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	mainProgram.useProgram();
	glBindVertexArray(mainVAO);

	mainProgram["matrices.projectionMatrix"] = getProjectionMatrix();
	mainProgram["matrices.viewMatrix"] = camera.getViewMatrix();

	// Render ground
	mainProgram["matrices.modelMatrix"] = glm::mat4(1.0);
	mainProgram["color"] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

	groundTexture.bind();
	samplersShowcase[groundSamplerIndex].bind();
	mainProgram["sampler"] = 0;

	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	const auto startPointLeft = glm::vec3(-40.0f, 0.0f, 75.0f);
	const auto startPointRight = glm::vec3(40.0f, 0.0f, 0.0f);
	const auto houseBottomSize = 10.0f;
	const auto roofTopSize = 12.0f;
	
	// Render "houses" on the left
	for (auto i = 0; i < 5; i++)
	{
		samplersShowcase[i].bind();
		// First, calculate the basic position of house
        
		auto modelMatrixHouse = glm::mat4(1.0);
		auto housePosition = startPointLeft + glm::vec3(0.0f, 0.0f, -i * 25.0f);
		modelMatrixHouse = glm::translate(modelMatrixHouse, housePosition);
        
		// Render bottom cube of the house
		glm::mat4 modelMatrixBottom = glm::translate(modelMatrixHouse, glm::vec3(0.0f, houseBottomSize / 2.0f, 0.0f));
		modelMatrixBottom = glm::scale(modelMatrixBottom, glm::vec3(houseBottomSize, houseBottomSize, houseBottomSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixBottom;
		mainProgram["color"] = i == groundSamplerIndex ? glm::vec4(0.25f, 0.8f, 1.0f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		houseTexture.bind();
		glDrawArrays(GL_TRIANGLES, 4, 18);
		glDrawArrays(GL_TRIANGLES, 28, 6);
		
		filterTextures[i].bind();
		glDrawArrays(GL_TRIANGLES, 22, 6);

		roofTexture.bind();
		auto translateTopY = houseBottomSize + roofTopSize / 2.0f - 0.25f;
		glm::mat4 modelMatrixTop = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateTopY, 0.0f));
		modelMatrixTop = glm::scale(modelMatrixTop, glm::vec3(roofTopSize, roofTopSize, roofTopSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixTop;
		glDrawArrays(GL_TRIANGLES, 40, 24);

		if(glm::distance(camera.getEye(), housePosition) < houseBottomSize*0.66f) {
			groundSamplerIndex = i;
		}
	}

	// Render "houses" on the right
	for (auto i = 5, j = 0; i < 10; i++, j++)
	{
		samplersShowcase[i].bind();

		// First, calculate the basic position of house 
		auto modelMatrixHouse = glm::mat4(1.0);
		auto housePosition = startPointRight + glm::vec3(0.0f, 0.0f, -j * 25.0f);
		modelMatrixHouse = glm::translate(modelMatrixHouse, housePosition);
        
		// Render bottom cube of the house
		glm::mat4 modelMatrixBottom = glm::translate(modelMatrixHouse, glm::vec3(0.0f, houseBottomSize / 2.0f, 0.0f));
		//	modelMatrixBottom = glm::rotate(modelMatrixBottom, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		modelMatrixBottom = glm::scale(modelMatrixBottom, glm::vec3(houseBottomSize, houseBottomSize, houseBottomSize));
		mainProgram["matrices.modelMatrix"] = modelMatrixBottom;
		mainProgram["color"] = i == groundSamplerIndex ? glm::vec4(0.25f, 0.8f, 1.0f, 1.0f) : glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);

		houseTexture.bind();
		glDrawArrays(GL_TRIANGLES, 4, 12);
		glDrawArrays(GL_TRIANGLES, 22, 12);

		filterTextures[i].bind();
		glDrawArrays(GL_TRIANGLES, 16, 6);

		roofTexture.bind();
        auto translateTopY = houseBottomSize + roofTopSize / 2.0f - 0.25f;
        glm::mat4 modelMatrixTop = glm::translate(modelMatrixHouse, glm::vec3(0.0f, translateTopY, 0.0f));
        modelMatrixTop = glm::scale(modelMatrixTop, glm::vec3(roofTopSize, roofTopSize, roofTopSize));
        mainProgram["matrices.modelMatrix"] = modelMatrixTop;
        glDrawArrays(GL_TRIANGLES, 40, 24);

		if (glm::distance(camera.getEye(), housePosition) < houseBottomSize*0.66f) {
			groundSamplerIndex = i;
		}
	}

	std::string windowTitleWithFPS = "007.) Textures pt. 1 - Single Texture - Tutorial by Michal Bubnar (www.mbsoftworks.sk) - FPS: "
		+ std::to_string(getFPS()) +
		", VSync: " + (isVerticalSynchronizationEnabled() ? "On" : "Off") + " (Press F3 to toggle)";
	glfwSetWindowTitle(getWindow(), windowTitleWithFPS.c_str());
}

void OpenGLWindow007::releaseScene()
{
	mainProgram.deleteProgram();

	vertexShader.deleteShader();
	fragmentShader.deleteShader();

	shapesVBO.deleteVBO();
	texCoordsVBO.deleteVBO();

	groundTexture.deleteTexture();
	houseTexture.deleteTexture();
	roofTexture.deleteTexture();

	for(auto i = 0; i < 10; i++)
	{
		filterTextures[i].deleteTexture();
		samplersShowcase[i].deleteSampler();
	}

	glDeleteVertexArrays(1, &mainVAO);
}

void OpenGLWindow007::updateScene()
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
}
