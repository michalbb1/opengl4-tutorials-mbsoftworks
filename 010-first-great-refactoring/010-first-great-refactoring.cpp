// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "010-first-great-refactoring.h"

#include "../common_classes/shader.h"
#include "../common_classes/shaderProgram.h"
#include "../common_classes/vertexBufferObject.h"
#include "../common_classes/staticGeometry.h"
#include "../common_classes/flyingCamera.h"
#include "../common_classes/texture.h"
#include "../common_classes/sampler.h"

#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/house.h"
#include "../common_classes/static_meshes_3D/snowCoveredPlainGround.h"

#include "HUD010.h"

FlyingCamera camera(glm::vec3(-120.0f, 8.0f, 120.0f), glm::vec3(-120.0f, 8.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);
std::unique_ptr<static_meshes_3D::House> house;
std::unique_ptr<static_meshes_3D::SnowCoveredPlainGround> snowCoveredPlainGround;
std::unique_ptr<HUD010> hud;

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

void OpenGLWindow010::initializeScene()
{
	glClearColor(0.0f, 0.28f, 0.57f, 1.0f);

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

		house = std::make_unique<static_meshes_3D::House>();
		snowCoveredPlainGround = std::make_unique<static_meshes_3D::SnowCoveredPlainGround>(true, true, true);
		
		hud = std::make_unique<HUD010>(*this);
		spm.linkAllPrograms();

		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
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

void OpenGLWindow010::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set matrices in matrix manager
	mm.setProjectionMatrix(getProjectionMatrix());
	mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
	mm.setViewMatrix(camera.getViewMatrix());

	snowCoveredPlainGround->render();

	// Render all houses
	auto& mainProgram = spm.getShaderProgram("main");
	mainProgram.useProgram();
	SamplerManager::getInstance().getSampler("main").bind();
	mainProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
	mainProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;

	for (auto& houseTransformation : houseTransformations)
	{
		house->render(houseTransformation.position, houseTransformation.angle);
	}

	// Render HUD
	hud->renderHUD();
}

void OpenGLWindow010::handleInput()
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
        hud->toggleBlending();
    }

    // Update window title
    std::string windowTitleWithFPS = std::string("010.) First Great Refactoring - Tutorial by Michal Bubnar (www.mbsoftworks.sk) - ")
        + "FPS: " + std::to_string(getFPS())
        + ", VSync: " + (isVerticalSynchronizationEnabled() ? "On" : "Off") + " (Press F3 to toggle)"
        + ", Blending: " + (hud->isBlendingEnabled() ? "On" : "Off") + " (Press F4 to toggle)";

    glfwSetWindowTitle(getWindow(), windowTitleWithFPS.c_str());
}

void OpenGLWindow010::releaseScene()
{
	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();

	house.reset();
	snowCoveredPlainGround.reset();
	hud.reset();
}

