// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>

// Project
#include "022-cylinder-and-sphere.h"
#include "HUD022.h"
#include "planet.h"
#include "spaceStation.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/freeTypeFontManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/skybox.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"

FlyingCamera camera(glm::vec3(-100.0f, 25.0f, 120.0f), glm::vec3(-100.0f, 25.0f, 119.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f);

std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::Cylinder> cylinder;
std::unique_ptr<HUD022> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.5f, 0.5f, 0.5f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::normalize(glm::vec3(-sqrt(2)/2, -sqrt(2)/2, 0.0f)), 0.5f);
shader_structs::Material dimMaterial(0.5f, 13.0f);
shader_structs::Material shinyMaterial(1.0f, 32.0f);
int sphereSlices = 20;
int sphereStacks = 20;

void OpenGLWindow022::initializeScene()
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

		sm.loadVertexShader("normals", "data/shaders/normals/normals.vert");
		sm.loadGeometryShader("normals", "data/shaders/normals/normals.geom");
		sm.loadFragmentShader("normals", "data/shaders/normals/normals.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut021_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut021_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));

		auto& normalsShaderProgram = spm.createShaderProgram("normals");
		normalsShaderProgram.addShaderToProgram(sm.getVertexShader("normals"));
		normalsShaderProgram.addShaderToProgram(sm.getGeometryShader("normals"));
		normalsShaderProgram.addShaderToProgram(sm.getFragmentShader("normals"));
		 
		skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/space_lightblue", "jpg");
		hud = std::make_unique<HUD022>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);

        tm.loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");

        tm.loadTexture2D("sun", "data/textures/solar_system/2k_sun.jpg");
        tm.loadTexture2D("mercury", "data/textures/solar_system/2k_mercury.jpg");
        tm.loadTexture2D("venus", "data/textures/solar_system/2k_venus_surface.jpg");
        tm.loadTexture2D("earth", "data/textures/solar_system/2k_earth_daymap.jpg");
        tm.loadTexture2D("mars", "data/textures/solar_system/2k_mars.jpg");
        tm.loadTexture2D("jupiter", "data/textures/solar_system/2k_jupiter.jpg");
        tm.loadTexture2D("saturn", "data/textures/solar_system/2k_saturn.jpg");
        tm.loadTexture2D("uranus", "data/textures/solar_system/2k_uranus.jpg");
        tm.loadTexture2D("neptune", "data/textures/solar_system/2k_neptune.jpg");

        Planet::initializeGeometry(sphereSlices, sphereStacks);
        SpaceStation::initializeGeometry();

		spm.linkAllPrograms();
	}
	catch (const std::runtime_error& ex)
	{
		std::cout << "Error occurred during initialization: " << ex.what() << std::endl;
		closeWindow(true);
		return;
	}

	glEnable(GL_DEPTH_TEST);
	glClearDepth(1.0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}

bool isDirectionLocked = true;
bool displayNormals = false;
bool updateSpaceEntities = true;

std::vector<Planet> planets =
{
    // The orbital periods of planets are actual periods taken from Wikipedia (in Earth days)
    Planet("sun", shinyMaterial, 60.0f, 0.0f, 0.0f, 0.05f, 0.0f), // I know that Sun isn't a planet :P
    Planet("mercury", dimMaterial, 8.0f, 105.0f, 87.9691f, 1.0f, 2.0f),
    Planet("venus", shinyMaterial, 18.0f, 160.0f, 224.701f, 0.5f, 2.5f),
    Planet("earth", dimMaterial, 20.0f, 240.0f, 365.0f, 1.0f, 4.0f),
    Planet("mars", dimMaterial, 13.0f, 320.0f, 686.971f, 0.7f, 1.0f),
    Planet("jupiter", dimMaterial, 47.0f, 450.0f, 4332.59f, 0.8f, 0.2f),
    Planet("saturn", dimMaterial, 35.0f, 570.0f, 10759.22f, 0.5f, 0.3f),
    Planet("uranus", shinyMaterial, 25.0f, 650.0f, 30688.5f, 0.6f, 0.4f),
    Planet("neptune", shinyMaterial, 24.0f, 730.0f, 60182.0f, 0.5f, 0.5f)
};

SpaceStation spaceStation(glm::vec3(100.0f, 100.0f, -100.0f), 15.0f, 3.0f);

void OpenGLWindow022::renderScene()
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
	shinyMaterial.setUniform(mainProgram, ShaderConstants::material());
    
    // Render space entities
    for (auto& planet : planets) {
        planet.render();
    }

    spaceStation.render();

	if (displayNormals)
	{
		// Set up some common properties in the normals shader program
		auto& normalsShaderProgram = spm.getShaderProgram("normals");
		normalsShaderProgram.useProgram();
		normalsShaderProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
		normalsShaderProgram[ShaderConstants::viewMatrix()] = camera.getViewMatrix();
		normalsShaderProgram[ShaderConstants::normalLength()] = 2.0f;

        // Render space entities normals
        for (auto& planet : planets) {
            planet.renderPoints();
        }

        spaceStation.renderPoints();
	}

    // Update space entities
    if (updateSpaceEntities)
    {
        for (auto& planet : planets) {
            planet.update();
        }

        spaceStation.update();
    }

	// Render HUD
	hud->renderHUD(diffuseLight.direction, displayNormals, updateSpaceEntities, sphereSlices, sphereStacks);
}

void OpenGLWindow022::updateScene()
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

    if (keyPressedOnce(GLFW_KEY_L)) {
        isDirectionLocked = !isDirectionLocked;
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
        updateSpaceEntities = !updateSpaceEntities;
    }

    if (keyPressedOnce(GLFW_KEY_1))
    {
        if (sphereSlices > 3)
        {
            sphereSlices--;
            Planet::initializeGeometry(sphereSlices, sphereStacks);
        }
    }

    if (keyPressedOnce(GLFW_KEY_2))
    {
        sphereSlices++;
        Planet::initializeGeometry(sphereSlices, sphereStacks);
    }

    if (keyPressedOnce(GLFW_KEY_3))
    {
        if (sphereStacks > 2)
        {
            sphereStacks--;
            Planet::initializeGeometry(sphereSlices, sphereStacks);
        }
    }

    if (keyPressedOnce(GLFW_KEY_4))
    {
        sphereStacks++;
        Planet::initializeGeometry(sphereSlices, sphereStacks);
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

void OpenGLWindow022::releaseScene()
{
	cylinder.reset();
	skybox.reset();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

    Planet::freeGeometry();
    SpaceStation::freeGeometry();

	hud.reset();
}

