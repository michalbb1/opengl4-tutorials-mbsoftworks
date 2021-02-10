// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "016-heightmap-pt1-random-terrain.h"
#include "HUD016.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/heightmap.h"
#include "../common_classes/static_meshes_3D/primitives/cube.h"
#include "../common_classes/static_meshes_3D/primitives/pyramid.h"
#include "../common_classes/static_meshes_3D/primitives/torus.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"

FlyingCamera camera(glm::vec3(0.0f, 10.0f, -60.0f), glm::vec3(0.0f, 10.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 15.0f);

std::unique_ptr<static_meshes_3D::Cube> cube;
std::unique_ptr<static_meshes_3D::Pyramid> pyramid;
std::unique_ptr<static_meshes_3D::Torus> torus;
std::unique_ptr<static_meshes_3D::Heightmap> heightmap;
std::unique_ptr<HUD016> hud;

float rotationAngleRad = 0.0f;
bool displayNormals = false;
shader_structs::AmbientLight ambientLight(glm::vec3(0.5f, 0.5f, 0.5f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)), 0.5f);
static_meshes_3D::Heightmap::HillAlgorithmParameters hillAlgorithmParams(
	200,  // Number of rows
	200,  // Number of columns
	250,  // Number of generated hills
	10,   // Minimal hill radius
	20,   // Maximal hill radius
	0.1f, // Minimal hill height
	0.2f  // Maximal hill height
);

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
	glm::vec3(30.0f, 0.0f, -80.0f),
	glm::vec3(-30.0f, 0.0f, -40.0f),
	glm::vec3(30.0f, 0.0f, 0.0f),
	glm::vec3(-30.0f, 0.0f, 40.0f),
	glm::vec3(30.0f, 0.0f, 80.0f)
};

void OpenGLWindow016::initializeScene()
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

		sm.loadVertexShader("normals", "data/shaders/normals/normals.vert");
		sm.loadGeometryShader("normals", "data/shaders/normals/normals.geom");
		sm.loadFragmentShader("normals", "data/shaders/normals/normals.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut014_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut014_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("ambientLight"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("diffuseLight"));

		auto& normalsShaderProgram = spm.createShaderProgram("normals");
		normalsShaderProgram.addShaderToProgram(sm.getVertexShader("normals"));
		normalsShaderProgram.addShaderToProgram(sm.getGeometryShader("normals"));
		normalsShaderProgram.addShaderToProgram(sm.getFragmentShader("normals"));

		hud = std::make_unique<HUD016>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("clay", "data/textures/clay.png");
		TextureManager::getInstance().loadTexture2D("crate", "data/textures/crate.png");
		TextureManager::getInstance().loadTexture2D("white_marble", "data/textures/white_marble.jpg");
		
		cube = std::make_unique<static_meshes_3D::Cube>(true, true, true);
		pyramid = std::make_unique<static_meshes_3D::Pyramid>(true, true, true);
		torus = std::make_unique<static_meshes_3D::Torus>(20, 20, 3.0f, 1.5f, true, true, true);

		heightmap = std::make_unique<static_meshes_3D::Heightmap>(hillAlgorithmParams, true, true, true);

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

const glm::vec3 heightMapSize(200.0f, 10.0f, 200.0f);

void getHeightmapRowAndColumn(const glm::vec3& position, int& row, int& column)
{
	const auto halfWidth = heightMapSize.x / 2.0f;
	const auto halfDepth = heightMapSize.z / 2.0f;

	row = int(heightmap->getRows() * (position.z + halfDepth) / heightMapSize.z);
	column = int(heightmap->getColumns() * (position.x + halfWidth) / heightMapSize.x);
}

void OpenGLWindow016::renderScene()
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
	
	// Set up ambient light in the shader program
	ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	// Set up diffuse light
	diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());

	// Render heightmap
	const auto heightmapModelMatrix = glm::scale(glm::mat4(1.0f), heightMapSize);
	mainProgram.setModelAndNormalMatrix(heightmapModelMatrix);
	TextureManager::getInstance().getTexture("clay").bind(0);
	heightmap->render();

	// Render all the crates (as simple cubes)
	std::vector<glm::mat4> crateModelMatrices;
	for (const auto& position : cratePositions)
	{
		const auto crateSize = 8.0f;
		auto model = glm::translate(glm::mat4(1.0f), position);
		int row = 0, column = 0;
		getHeightmapRowAndColumn(position, row, column);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f + crateSize / 2.0f + heightmap->getHeight(row, column)*heightMapSize.y, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(1.0f, 0.0f, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(0.0f, 1.0f, 0.0f));
		model = glm::rotate(model, rotationAngleRad, glm::vec3(0.0f, 0.0f, 1.0f));
		model = glm::scale(model, glm::vec3(crateSize, crateSize, crateSize));
		crateModelMatrices.push_back(model);
		mainProgram.setModelAndNormalMatrix(model);

		TextureManager::getInstance().getTexture("crate").bind(0);
		cube->render();
	}

	// Render tori
	std::vector<glm::mat4> torusModelMatrices;
	for (const auto& position : toriPositions)
	{
		auto model = glm::translate(glm::mat4(1.0f), position);
		int row = 0, column = 0;
		getHeightmapRowAndColumn(position, row, column);
		model = glm::translate(model, glm::vec3(0.0f, 4.5f + heightmap->getHeight(row, column)*heightMapSize.y, 0.0f));
		model = glm::rotate(model, rotationAngleRad + 90.0f, glm::vec3(0.0f, 1.0f, 0.0f));
		torusModelMatrices.push_back(model);
		mainProgram.setModelAndNormalMatrix(model);

		TextureManager::getInstance().getTexture("white_marble").bind(0);
		torus->render();
	}

	if (displayNormals)
	{
		// Set up some common properties in the normals shader program
		auto& normalsShaderProgram = spm.getShaderProgram("normals");
		normalsShaderProgram.useProgram();
		normalsShaderProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
		normalsShaderProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
		normalsShaderProgram[ShaderConstants::normalLength()] = 0.5f;

		// Render all the crates points
		auto matrixIt = crateModelMatrices.cbegin();
		for (const auto& position : cratePositions)
		{
			normalsShaderProgram.setModelAndNormalMatrix(*matrixIt++);
			cube->renderPoints();
		}

		// Finally render tori points
		matrixIt = torusModelMatrices.cbegin();
		for (const auto& position : toriPositions)
		{
			normalsShaderProgram.setModelAndNormalMatrix(*matrixIt++);
			torus->renderPoints();
		}

		normalsShaderProgram.setModelAndNormalMatrix(heightmapModelMatrix);
		heightmap->renderPoints();
	}

	// Render HUD
	hud->renderHUD(displayNormals);
}

void OpenGLWindow016::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_N)) {
        displayNormals = !displayNormals;
    }

    if (keyPressedOnce(GLFW_KEY_R)) {
        heightmap = std::make_unique<static_meshes_3D::Heightmap>(hillAlgorithmParams, true, true, true);
    }

    int posX, posY, width, height;
    glfwGetWindowPos(getWindow(), &posX, &posY);
    glfwGetWindowSize(getWindow(), &width, &height);
    camera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));

    camera.update([this](int keyCode) {return this->keyPressed(keyCode); },
        [this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
        [this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
        [this](float f) {return this->sof(f); });

    // Update rotation angle
    rotationAngleRad += sof(glm::radians(45.0f));
}

void OpenGLWindow016::releaseScene()
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
	heightmap.reset();
}
