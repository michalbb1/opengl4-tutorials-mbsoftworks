#include <iostream>
#include <memory>

#include <glm/gtc/matrix_transform.hpp>

#include "../common_classes/OpenGLWindow.h"
#include "../common_classes/flyingCamera.h"

#include "../common_classes/freetypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/skybox.h"
#include "../common_classes/static_meshes_3D/heightmap.h"
#include "../common_classes/static_meshes_3D/assimpModel.h"

#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"

#include "HUD021.h"

FlyingCamera camera(glm::vec3(0.0f, 25.0f, -60.0f), glm::vec3(0.0f, 25.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f);

std::unique_ptr<static_meshes_3D::Heightmap> heightmapWithFog;
std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<HUD021> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.5f, 0.5f, 0.5f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(-sqrt(2)/2, -sqrt(2)/2, 0.0f)), 0.5f);
shader_structs::Material material(1.0f, 32.0f);

std::vector<glm::vec3> tripleToriPositions
{
	glm::vec3(-80.0f, 0.0f, -80.0f),
	glm::vec3(80.0f, 0.0f, -80.0f),
	glm::vec3(-5.0f, 0.0f, 0.0f),
	glm::vec3(-80.0f, 0.0f, 80.0f),
	glm::vec3(82.0f, 0.0f, 75.0f)
};

void OpenGLWindow::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut021_main", "data/shaders/tut021-specular-highlight/shader.vert");
		sm.loadFragmentShader("tut021_main", "data/shaders/tut021-specular-highlight/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
		sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
		sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
		sm.loadFragmentShader(ShaderKeys::utility(), "data/shaders/common/utility.frag");

		sm.loadVertexShader("normals", "data/shaders/normals/normals.vert");
		sm.loadGeometryShader("normals", "data/shaders/normals/normals.geom");
		sm.loadFragmentShader("normals", "data/shaders/normals/normals.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut021_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut021_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::utility()));
		 
		skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajsnow1", "jpg");
		hud = std::make_unique<HUD021>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("grass", "data/textures/grass.jpg");
		TextureManager::getInstance().loadTexture2D("rocky_terrain", "data/textures/rocky_terrain.jpg");
		TextureManager::getInstance().loadTexture2D("snow", "data/textures/snow.png");
		TextureManager::getInstance().loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");
		
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 8.0f, 2.0f, true, true, true);

		static_meshes_3D::Heightmap::prepareMultiLayerShaderProgram();
		heightmapWithFog = std::make_unique<static_meshes_3D::Heightmap>("data/heightmaps/tut019.png", true, true, true);

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
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

const glm::vec3 heightMapSize(200.0f, 40.0f, 200.0f);
bool isDirectionLocked = true;
float rotationAngle;

void getHeightmapRowAndColumn(const glm::vec3& position, int& row, int& column)
{
	const auto halfWidth = heightMapSize.x / 2.0f;
	const auto halfDepth = heightMapSize.z / 2.0f;

	row = int(heightmapWithFog->getRows() * (position.z + halfDepth) / heightMapSize.z);
	column = int(heightmapWithFog->getColumns() * (position.x + halfWidth) / heightMapSize.x);
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
	mainProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
	mainProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
	mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;
	
	// Render skybox first with only ambient light
	shader_structs::AmbientLight(glm::vec3(0.8f, 0.8f, 0.8f), true).setUniform(mainProgram, ShaderConstants::ambientLight());
	shader_structs::DiffuseLight::none().setUniform(mainProgram, ShaderConstants::diffuseLight());
	shader_structs::Material::none().setUniform(mainProgram, ShaderConstants::material());
	skybox->render(camera.getEye(), mainProgram);

	// Set up ambient and diffuse light in the shader program
	if (!isDirectionLocked) {
		diffuseLight.direction = glm::normalize(camera.getViewPoint() - camera.getEye());
	}
	SamplerManager::getInstance().getSampler("main").bind();
	ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());

	// Set up material properties for specular highlights and apply scifi metal texture
	mainProgram[ShaderConstants::eyePosition()] = camera.getEye();
	material.setUniform(mainProgram, ShaderConstants::material());
	TextureManager::getInstance().getTexture("scifi_metal").bind(0);

	// Render triple tori on their positions
	int i = 0;
	for (const auto& position : tripleToriPositions)
	{
		int row = 0, column = 0;
		getHeightmapRowAndColumn(position, row, column);
		const auto translateY = heightmapWithFog->getHeight(row, column)*heightMapSize.y + 11.0f;

		auto basicModelMatrix = glm::translate(glm::mat4(1.0f), position);
		basicModelMatrix = glm::translate(basicModelMatrix, glm::vec3(0.0f, translateY, 0.0f));
		basicModelMatrix = glm::rotate(basicModelMatrix, rotationAngle, glm::vec3(0.0f, 1.0f, 0.0f));

		// Render first torus with the calculated model matrix
		mainProgram.setModelAndNormalMatrix(basicModelMatrix);
		torus->render();

		// Rotate by 90 degrees around Y axis and render torus again
		// Also scale slightly, so that the tori won't overlap
		const auto secondScaleVector = i % 2 == 0 ? glm::vec3(1.01f, 1.01f, 1.01f) : glm::vec3(0.6f, 0.6f, 0.6f);
		auto secondToriModelMatrix = glm::scale(basicModelMatrix, secondScaleVector);
		secondToriModelMatrix = glm::rotate(secondToriModelMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		mainProgram.setModelAndNormalMatrix(secondToriModelMatrix);
		torus->render();

		// Rotate by 90 degrees around X axis and render torus again
		// Also scale slightly, so that the tori won't overlap
		const auto thirdScaleVector = i % 2 == 0 ? glm::vec3(1.02f, 1.02f, 1.02f) : glm::vec3(0.36f, 0.36f, 0.36f);
		auto thirdToriModelMatrix = glm::scale(basicModelMatrix, thirdScaleVector);
		thirdToriModelMatrix = glm::rotate(thirdToriModelMatrix, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		mainProgram.setModelAndNormalMatrix(thirdToriModelMatrix);
		torus->render();

		i++;
	}

	// Render heightmap
	auto& heightmapShaderProgram = static_meshes_3D::Heightmap::getMultiLayerShaderProgram();
	heightmapShaderProgram.useProgram();
	heightmapShaderProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
	heightmapShaderProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
	heightmapShaderProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	heightmapShaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ambientLight.setUniform(heightmapShaderProgram, ShaderConstants::ambientLight());
	diffuseLight.setUniform(heightmapShaderProgram, ShaderConstants::diffuseLight());

	const auto heightmapModelMatrix = glm::scale(glm::mat4(1.0f), heightMapSize);
	heightmapShaderProgram.setModelAndNormalMatrix(heightmapModelMatrix);
	heightmapWithFog->renderMultilayered({"snow", "rocky_terrain", "snow"}, {0.2f, 0.3f, 0.55f, 0.7f});

	// Render HUD
	hud->renderHUD(material, diffuseLight.direction);
	
	// Update variables
	rotationAngle += sof(glm::radians(30.0f));
}

void OpenGLWindow::releaseScene()
{
	torus.reset();
	skybox.reset();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	hud.reset();
	heightmapWithFog.reset();
}

void OpenGLWindow::handleInput()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

	if (keyPressedOnce(GLFW_KEY_F3)) {
		setVerticalSynchronization(!isVerticalSynchronizationEnabled());
	}

	if (keyPressedOnce(GLFW_KEY_X)) {
		material.isEnabled = !material.isEnabled;
	}

	if (keyPressed(GLFW_KEY_1)) {
		material.specularIntensity -= sof(0.5f);
	}

	if (keyPressed(GLFW_KEY_2)) {
		material.specularIntensity += sof(0.5f);
	}

	if (keyPressed(GLFW_KEY_3)) {
		material.specularPower -= sof(10.0f);
	}

	if (keyPressed(GLFW_KEY_4)) {
		material.specularPower += sof(10.0f);
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
