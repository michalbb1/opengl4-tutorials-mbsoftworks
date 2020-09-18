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
#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"

#include "HUD014.h"

FlyingCamera camera(glm::vec3(0.0f, 10.0f, -60.0f), glm::vec3(0.0f, 10.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);

std::unique_ptr<static_meshes_3D::Cube> cube;
std::unique_ptr<static_meshes_3D::Pyramid> pyramid;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<static_meshes_3D::PlainGround> plainGround;
std::unique_ptr<HUD014> hud;

float rotationAngleRad = 0.0f;
bool isDirectionLocked = false;
shader_structs::AmbientLight ambientLight(glm::vec3(0.25f, 0.25f, 0.25f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.75f);

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
	glm::vec3(30.0f, 0.0f, -40.0f),
	glm::vec3(-30.0f, 0.0f, 0.0f),
	glm::vec3(30.0f, 0.0f, 40.0f),
	glm::vec3(-30.0f, 0.0f, 80.0f),
};

std::vector<glm::vec3> toriPositions
{
	glm::vec3(30.0f, 5.0f, -80.0f),
	glm::vec3(-30.0f, 5.0f, -40.0f),
	glm::vec3(30.0f, 5.0f, 0.0f),
	glm::vec3(-30.0f, 5.0f, 40.0f),
	glm::vec3(30.0f, 5.0f, 80.0f)
};

void OpenGLWindow::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut014_main", "data/shaders/tut014-diffuse-lighting/shader.vert");
		sm.loadFragmentShader("tut014_main", "data/shaders/tut014-diffuse-lighting/shader.frag");
		sm.loadFragmentShader("ambientLight", "data/shaders/lighting/ambientLight.frag");
		sm.loadFragmentShader("diffuseLight", "data/shaders/lighting/diffuseLight.frag");
		sm.loadFragmentShader("utility", "data/shaders/common/utility.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut014_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut014_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("ambientLight"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("diffuseLight"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("utility"));

		hud = std::make_unique<HUD014>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("grass", "data/textures/grass.jpg");
		TextureManager::getInstance().loadTexture2D("crate", "data/textures/crate.png");
		TextureManager::getInstance().loadTexture2D("rocky_terrain", "data/textures/rocky_terrain.jpg");
		TextureManager::getInstance().loadTexture2D("white_marble", "data/textures/white_marble.jpg");
		
		cube = std::make_unique<static_meshes_3D::Cube>(true, true, true);
		pyramid = std::make_unique<static_meshes_3D::Pyramid>(true, true, true);
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 3.0f, 1.5f, true, true, true);
		plainGround = std::make_unique<static_meshes_3D::PlainGround>(true, true, true);

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

	const auto clearColor = glm::vec4(0.02f, 0.682f, 1.0f, 1.0f)*glm::vec4(ambientLight.getColorContribution(), 1.0f);
	glClearColor(clearColor.r, clearColor.g, clearColor.b, clearColor.a);
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
	mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;
	
	// Set up ambient light in the shader programs
	ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());

	// Set up diffuse light and set the light's direction to the camera direction
	if (!isDirectionLocked) {
		diffuseLight.direction = glm::normalize(camera.getViewPoint() - camera.getEye());
	}
	diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());

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
		mainProgram.setModelAndNormalMatrix(model);

		TextureManager::getInstance().getTexture("crate").bind(0);
		cube->render();
	}

	// Proceed with rendering rocky pyramids
	for (const auto& position : pyramidPositions)
	{
		const auto pyramidSize = 10.0f;
		auto model = glm::translate(glm::mat4(1.0f), position);
		model = glm::translate(model, glm::vec3(0.0f, pyramidSize / 2.0f, 0.0f));
		model = glm::scale(model, glm::vec3(pyramidSize, pyramidSize, pyramidSize));
		mainProgram.setModelAndNormalMatrix(model);

		TextureManager::getInstance().getTexture("rocky_terrain").bind(0);
		pyramid->render();
	}

	// Finally render tori
	for (const auto& position : toriPositions)
	{
		auto model = glm::translate(glm::mat4(1.0f), position);
		model = glm::rotate(model, rotationAngleRad+90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		mainProgram.setModelAndNormalMatrix(model);

		TextureManager::getInstance().getTexture("white_marble").bind(0);
		torus->render();
	}

	// Render HUD
	hud->renderHUD(ambientLight, diffuseLight);

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

	if (keyPressed(GLFW_KEY_1)) {
		ambientLight.color -= glm::vec3(sof(0.25f));
		if (ambientLight.color.r < 0.0f) {
			ambientLight.color = glm::vec3(0.0f);
		}
	}

	if (keyPressed(GLFW_KEY_2)) {
		ambientLight.color += glm::vec3(sof(0.25f));
		if (ambientLight.color.r > 1.0f) {
			ambientLight.color = glm::vec3(1.0f);
		}
	}

	if (keyPressed(GLFW_KEY_3)) {
		diffuseLight.factor -= sof(0.25f);
		if (diffuseLight.factor < 0.0f) {
			diffuseLight.factor = 0.0f;
		}
	}

	if (keyPressed(GLFW_KEY_4)) {
		diffuseLight.factor += sof(0.25f);
		if (diffuseLight.factor > 1.0f) {
			diffuseLight.factor = 1.0f;
		}
	}

	if (keyPressedOnce(GLFW_KEY_Z)) {
		ambientLight.isOn = !ambientLight.isOn;
	}

	if (keyPressedOnce(GLFW_KEY_X)) {
		diffuseLight.isOn = !diffuseLight.isOn;
	}

	if (keyPressedOnce(GLFW_KEY_L)) {
		isDirectionLocked = !isDirectionLocked;
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
