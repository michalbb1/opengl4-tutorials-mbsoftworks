// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "020-fog.h"
#include "HUD020.h"
#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/skybox.h"
#include "../common_classes/static_meshes_3D/heightmapWithFog.h"
#include "../common_classes/static_meshes_3D/assimpModel.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/fogParameters.h"

namespace opengl4_mbsoftworks {
namespace tutorial020 {

FlyingCamera camera(glm::vec3(0.0f, 25.0f, -60.0f), glm::vec3(0.0f, 25.0f, -59.0f), glm::vec3(0.0f, 1.0f, 0.0f), 25.0f);

std::unique_ptr<static_meshes_3D::HeightmapWithFog> heightmapWithFog;
std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::AssimpModel> medievalHouseModel;
std::unique_ptr<static_meshes_3D::AssimpModel> classicHouseModel;
std::unique_ptr<HUD020> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.6f, 0.6f, 0.6f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(0.0f, -1.0f, -1.0f)), 0.4f);
shader_structs::FogParameters fogParameters(glm::vec3(0.7f, 0.7f, 0.7f), 20.0f, 75.0f, 0.01f, shader_structs::FogParameters::FOG_EQUATION_EXP2);

std::vector<glm::vec3> classicHousePositions
{
	glm::vec3(80.0f, 0.0f, -80.0f),
	glm::vec3(-80.0f, 0.0f, 80.0f)
};

std::vector<glm::vec3> medievalHousePositions
{
	glm::vec3(-80.0f, 0.0f, -80.0f),
	glm::vec3(82.0f, 0.0f, 75.0f),
	glm::vec3(-5.0f, 0.0f, 00.0f)
};

const glm::vec3 heightMapSize(200.0f, 40.0f, 200.0f);

void OpenGLWindow020::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

		sm.loadVertexShader("tut020_fog_main", "data/shaders/tut020-fog/shader.vert");
		sm.loadFragmentShader("tut020_fog_main", "data/shaders/tut020-fog/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
		sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
		sm.loadFragmentShader(ShaderKeys::fog(), "data/shaders/fog/fog.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut020_fog_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut020_fog_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::fog()));
		
		skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajsnow1", "jpg");
		hud = std::make_unique<HUD020>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
		TextureManager::getInstance().loadTexture2D("grass", "data/textures/grass.jpg");
		TextureManager::getInstance().loadTexture2D("rocky_terrain", "data/textures/rocky_terrain.jpg");
		TextureManager::getInstance().loadTexture2D("snow", "data/textures/snow.png");
		
		classicHouseModel = std::make_unique<static_meshes_3D::AssimpModel>("data/models/house/house.3ds", true, true, true);
		medievalHouseModel = std::make_unique<static_meshes_3D::AssimpModel>("data/models/medieval_house/medieval_house.obj", "medieval_house_diff.png", true, true, true);

		static_meshes_3D::HeightmapWithFog::prepareMultiLayerShaderProgramWithFog();
		heightmapWithFog = std::make_unique<static_meshes_3D::HeightmapWithFog>("data/heightmaps/tut019.png", true, true, true);

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

void OpenGLWindow020::renderScene()
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
	fogParameters.setUniform(mainProgram, ShaderConstants::fogParams());
	skybox->render(camera.getEye(), mainProgram);

	// Set up ambient and diffuse light in the shader program
	SamplerManager::getInstance().getSampler("main").bind();
	ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());

	// Set up fog parameters in the shader program
	fogParameters.setUniform(mainProgram, ShaderConstants::fogParams());

	// Render classic houses
	for (const auto& position : classicHousePositions)
	{
		auto model = glm::translate(glm::mat4(1.0f), position);
        const auto renderedHeight = heightmapWithFog->getRenderedHeightAtPosition(heightMapSize, position);
		model = glm::translate(model, glm::vec3(0.0f, renderedHeight, 0.0f));
		model = glm::scale(model, glm::vec3(5.0f, 5.0f, 5.0f));

		// Render model with the calculated model matrix
		mainProgram.setModelAndNormalMatrix(model);
		classicHouseModel->render();
	}

	// Render medieval houses
	for (const auto& position : medievalHousePositions)
	{
		auto model = glm::translate(glm::mat4(1.0f), position);
        const auto renderedHeight = heightmapWithFog->getRenderedHeightAtPosition(heightMapSize, position);
		model = glm::translate(model, glm::vec3(0.0f, renderedHeight, 0.0f));
		model = glm::scale(model, glm::vec3(0.1f, 0.1f, 0.1f));

		// Render model with the calculated model matrix
		mainProgram.setModelAndNormalMatrix(model);
		medievalHouseModel->render();
	}

	// Render heightmap
	auto& heightmapShaderProgram = static_meshes_3D::HeightmapWithFog::getMultiLayerShaderProgramWithFog();
	heightmapShaderProgram.useProgram();
	heightmapShaderProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
	heightmapShaderProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
	heightmapShaderProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	heightmapShaderProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	ambientLight.setUniform(heightmapShaderProgram, ShaderConstants::ambientLight());
	diffuseLight.setUniform(heightmapShaderProgram, ShaderConstants::diffuseLight());
	fogParameters.setUniform(heightmapShaderProgram, ShaderConstants::fogParams());

	const auto heightmapModelMatrix = glm::scale(glm::mat4(1.0f), heightMapSize);
	heightmapShaderProgram.setModelAndNormalMatrix(heightmapModelMatrix);
	heightmapWithFog->renderMultilayered({"snow", "rocky_terrain", "snow"}, {0.2f, 0.3f, 0.55f, 0.7f});

	// Render HUD
	hud->renderHUD(fogParameters);
}

void OpenGLWindow020::updateScene()
{
    if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
        closeWindow();
    }

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_F)) {
        fogParameters.equation = (fogParameters.equation + 1) % 3;
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
        fogParameters.isEnabled = !fogParameters.isEnabled;
    }

    if (fogParameters.equation == shader_structs::FogParameters::FOG_EQUATION_LINEAR)
    {
        if (keyPressed(GLFW_KEY_1)) {
            fogParameters.linearStart -= sof(10.0f);
        }

        if (keyPressed(GLFW_KEY_2)) {
            fogParameters.linearStart += sof(10.0f);
        }

        if (keyPressed(GLFW_KEY_3)) {
            fogParameters.linearEnd -= sof(10.0f);
        }

        if (keyPressed(GLFW_KEY_4)) {
            fogParameters.linearEnd += sof(10.0f);
        }
    }
    else
    {
        if (keyPressed(GLFW_KEY_1)) {
            fogParameters.density -= sof(0.02f);
        }

        if (keyPressed(GLFW_KEY_2)) {
            fogParameters.density += sof(0.02f);
        }
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

void OpenGLWindow020::releaseScene()
{
	skybox.reset();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	classicHouseModel.reset();
	medievalHouseModel.reset();

	hud.reset();
	heightmapWithFog.reset();
}

} // namespace tutorial020
} // namespace opengl4_mbsoftworks