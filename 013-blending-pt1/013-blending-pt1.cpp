#include <iostream>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/plainGround.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/primitives/pyramid.h"

#include "HUD013.h"

FlyingCamera camera(glm::vec3(0.0f, 10.0f, -60.0f), glm::vec3(0.0f, 10.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);

std::unique_ptr<static_meshes_3D::Cube> cube;
std::unique_ptr<static_meshes_3D::Pyramid> pyramid;
std::unique_ptr<static_meshes_3D::PlainGround> plainGround;
std::unique_ptr<HUD013> hud;

bool turnDepthMaskOff = true;
float rotationAngleRad = 0.0f;
std::vector<glm::vec3> pyramidPositions
{
	glm::vec3(0.0f, 0.0f, -100.0f),
	glm::vec3(0.0f, 0.0f, -60.0f),
	glm::vec3(0.0f, 0.0f, -20.0f),
	glm::vec3(0.0f, 0.0f, 20.0f),
	glm::vec3(0.0f, 0.0f, 60.0f),
};

std::vector<glm::vec3> cratePositions
{
	glm::vec3(-30.0f, 0.0f, -80.0f),
	glm::vec3(-30.0f, 0.0f, -40.0f),
	glm::vec3(-30.0f, 0.0f, 0.0f),
	glm::vec3(-30.0f, 0.0f, 40.0f),
	glm::vec3(-30.0f, 0.0f, 80.0f),
	glm::vec3(30.0f, 0.0f, -80.0f),
	glm::vec3(30.0f, 0.0f, -40.0f),
	glm::vec3(30.0f, 0.0f, 0.0f),
	glm::vec3(30.0f, 0.0f, 40.0f),
	glm::vec3(30.0f, 0.0f, 80.0f)
};


void OpenGLWindow::initializeScene()
{
	glClearColor(0.02f, 0.682f, 1.0f, 1.0f);
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut007_main", "data/shaders/tut007/shader.vert");
		sm.loadFragmentShader("tut007_main", "data/shaders/tut007/shader.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut007_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut007_main"));

		hud = std::make_unique<HUD013>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("grass", "data/textures/grass.jpg");
		TextureManager::getInstance().loadTexture2D("crate", "data/textures/crate.png");
		TextureManager::getInstance().loadTexture2D("diamond", "data/textures/diamond.png");
		
		cube = std::make_unique<static_meshes_3D::Cube>(true, true, false);
		pyramid = std::make_unique<static_meshes_3D::Pyramid>(true, true, false);
		plainGround = std::make_unique<static_meshes_3D::PlainGround>(true, true, false);

		spm.linkAllPrograms();
	}
	catch (const std::runtime_error& ex)
	{
		std::cout << "Error occured during initialization: " << ex.what() << std::endl;
		closeWindow(true);
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
}

void OpenGLWindow::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set matrices in matrix manager
	mm.setProjectionMatrix(getProjectionMatrix());
	mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
	mm.setViewMatrix(camera.getViewMatrix());

	// Set up some common properties in the main shader program
	auto& mainProgram = spm.getShaderProgram("main");
	mainProgram.useProgram();
	SamplerManager::getInstance().getSampler("main").bind();
	mainProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
	mainProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
	mainProgram[ShaderConstants::modelMatrix()] = glm::mat4(1.0f);
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;

	// Render grass ground
	TextureManager::getInstance().getTexture("grass").bind(0);
	plainGround->render();

	// Render all the crates (as simple cubes)
	for (const auto& position : cratePositions)
	{
		const auto crateSize = 8.0f;
		auto model = glm::translate(glm::mat4(1.0f), position);
		model = glm::translate(model, glm::vec3(0.0f, crateSize / 2.0f + 3.0f, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(crateSize, crateSize, crateSize));
		mainProgram[ShaderConstants::modelMatrix()] = model;

		TextureManager::getInstance().getTexture("crate").bind(0);
		cube->render();
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Turn also depth mask back off, if it's desired
	if (turnDepthMaskOff) {
		glDepthMask(0);
	}
	
	// Change the color of diamond pyramids to white with 50% transparency (alpha is 0.5)
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 0.5f);

	// Proceed with rendering diamond pyramids
	for (const auto& position : pyramidPositions)
	{
		const auto pyramidSize = 10.0f;
		auto model = glm::translate(glm::mat4(1.0f), position);
		model = glm::translate(model, glm::vec3(0.0f, pyramidSize / 2.0f, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(pyramidSize, pyramidSize, pyramidSize));
		
		mainProgram[ShaderConstants::modelMatrix()] = model;

		TextureManager::getInstance().getTexture("diamond").bind(0);
		pyramid->render();
	}

	// Turn depth mask back on, if it has been turned off
	if (turnDepthMaskOff) {
		glDepthMask(1);
	}
	glDisable(GL_BLEND);

	// Render HUD
	hud->renderHUD(turnDepthMaskOff);

	// Update rotation angle
	rotationAngleRad += sof(glm::radians(45.0f));
}

void OpenGLWindow::releaseScene()
{
	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	hud.reset();
	pyramid.reset();
	cube.reset();
	plainGround.reset();
}

void OpenGLWindow::handleInput()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

	if (keyPressedOnce(GLFW_KEY_F3)) {
		setVerticalSynchronization(!isVerticalSynchronizationEnabled());
	}

	if (keyPressedOnce(GLFW_KEY_F4)) {
		turnDepthMaskOff = !turnDepthMaskOff;
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

void OpenGLWindow::onWindowSizeChanged(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}
