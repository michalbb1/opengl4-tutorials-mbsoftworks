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
#include "../common_classes/static_meshes_3D/primitives/pyramid.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "HUD012.h"

FlyingCamera camera(glm::vec3(0.0f, 10.0f, -60.0f), glm::vec3(0.0f, 10.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);

std::unique_ptr<static_meshes_3D::Pyramid> pyramid;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<static_meshes_3D::PlainGround> plainGround;
std::unique_ptr<HUD012> hud;

float rotationAngleRad = 0.0f;
std::vector<glm::vec3> objectPositions
{
	glm::vec3(-30.0f, 0.0f, -50.0f),
	glm::vec3(-30.0f, 0.0f, -25.0f),
	glm::vec3(-30.0f, 0.0f, 0.0f),
	glm::vec3(-30.0f, 0.0f, 25.0f),
	glm::vec3(-30.0f, 0.0f, 50.0f),
	glm::vec3(30.0f, 0.0f, -50.0f),
	glm::vec3(30.0f, 0.0f, -25.0f),
	glm::vec3(30.0f, 0.0f, 0.0f),
	glm::vec3(30.0f, 0.0f, 25.0f),
	glm::vec3(30.0f, 0.0f, 50.0f)
};


void OpenGLWindow::initializeScene()
{
	glClearColor(0.18f, 0.0f, 0.356f, 1.0f);
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

		hud = std::make_unique<HUD012>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("diamond", "data/textures/diamond.png");
		TextureManager::getInstance().loadTexture2D("metal", "data/textures/metal.png");
		TextureManager::getInstance().loadTexture2D("ice", "data/textures/ice.png");

		pyramid = std::make_unique<static_meshes_3D::Pyramid>(true, true, false);
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 3.0f, 1.5f, true, true, false);
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

	// Render icy ground
	TextureManager::getInstance().getTexture("ice").bind(0);
	plainGround->render();

	for (const auto& position : objectPositions)
	{
		// Render diamond pyramid on bottom
		const auto pyramidSize = 10.0f;
		auto posModelMatrix = glm::translate(glm::mat4(1.0f), position);
		auto model = glm::translate(posModelMatrix, glm::vec3(0.0f, pyramidSize / 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(pyramidSize, pyramidSize, pyramidSize));
		mainProgram[ShaderConstants::modelMatrix()] = model;

		TextureManager::getInstance().getTexture("diamond").bind(0);
		pyramid->render();

		// Render metal torus on top of the pyramid
		const auto torusOffset = pyramidSize + torus->getMainRadius() + torus->getTubeRadius();
		model = glm::translate(posModelMatrix, glm::vec3(0.0f, torusOffset, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		mainProgram[ShaderConstants::modelMatrix()] = model;

		TextureManager::getInstance().getTexture("metal").bind(0);
		torus->render();
	}

	// Render HUD
	hud->renderHUD();

	// Update rotation angle
	rotationAngleRad += sof(glm::radians(135.0f));
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
	torus.reset();
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
