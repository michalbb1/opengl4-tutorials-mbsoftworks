// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// Project
#include "027-occlusion-query.h"
#include "HUD027.h"
#include "objectsWithOccluderManager.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/skybox.h"
#include "../common_classes/static_meshes_3D/snowCoveredPlainGround.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"

FlyingCamera flyingCamera(glm::vec3(0.0f, 15.0f, 150.0f), glm::vec3(0.0f, 15.0f, 149.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f);

std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<static_meshes_3D::PlainGround> plainGround;

std::unique_ptr<HUD027> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.25f, 0.25f, 0.25f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, 0.0f, -1.0f), 0.75f);

bool visualizeOccluders = false;
bool isWireframeModeOn = false;

void OpenGLWindow027::initializeScene()
{
	try
	{
		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

        sm.loadVertexShader("tut027_main", "data/shaders/tut021-specular-highlight/shader.vert");
        sm.loadFragmentShader("tut027_main", "data/shaders/tut021-specular-highlight/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
        sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
        sm.loadVertexShader("single-color", "data/shaders/single-color/shader.vert");
        sm.loadFragmentShader("single-color", "data/shaders/single-color/shader.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut027_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut027_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));

        auto& singleColorShaderProgram = spm.createShaderProgram("single-color");
        singleColorShaderProgram.addShaderToProgram(sm.getVertexShader("single-color"));
        singleColorShaderProgram.addShaderToProgram(sm.getFragmentShader("single-color"));
		
        skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajlands1", "jpg");
		hud = std::make_unique<HUD027>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
        tm.loadTexture2D("grass", "data/textures/grass.jpg");
        tm.loadTexture2D("crate", "data/textures/crate.png");
        tm.loadTexture2D("white_marble", "data/textures/white_marble.jpg");
        tm.loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");
		
        plainGround = std::make_unique<static_meshes_3D::PlainGround>(true, true, true);
        ObjectsWithOccludersManager::getInstance().initialize();

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

void OpenGLWindow027::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Set matrices in matrix manager
	mm.setProjectionMatrix(getProjectionMatrix());
	mm.setOrthoProjectionMatrix(getOrthoProjectionMatrix());
	mm.setViewMatrix(flyingCamera.getViewMatrix());

	// Set up some common properties in the main shader program
	auto& mainProgram = spm.getShaderProgram("main");
	mainProgram.useProgram();
    mainProgram[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
    mainProgram[ShaderConstants::viewMatrix()] = flyingCamera.getViewMatrix();
	mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
	mainProgram[ShaderConstants::color()] = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
	mainProgram[ShaderConstants::sampler()] = 0;

    // Render skybox first with only ambient light and no material
	shader_structs::AmbientLight(glm::vec3(0.8f, 0.8f, 0.8f), true).setUniform(mainProgram, ShaderConstants::ambientLight());
	shader_structs::DiffuseLight::none().setUniform(mainProgram, ShaderConstants::diffuseLight());
    shader_structs::Material::none().setUniform(mainProgram, ShaderConstants::material());
	skybox->render(flyingCamera.getEye(), mainProgram);
	
	// After rendering skybox, set ambient and diffuse light from our variables
    mainProgram[ShaderConstants::eyePosition()] = flyingCamera.getEye();
    ambientLight.setUniform(mainProgram, ShaderConstants::ambientLight());
	diffuseLight.setUniform(mainProgram, ShaderConstants::diffuseLight());

    // Also set our main sampler, that doesn't clamp textures unlike skybox
    SamplerManager::getInstance().getSampler("main").bind();

    // Render grass ground
    tm.getTexture("grass").bind(0);
    mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
    plainGround->render();

    // Now before rendering, let's update all objects (generate new, remove some that flew high) and perform occlusion query
    auto& objectsWithOccludersManager = ObjectsWithOccludersManager::getInstance();
    objectsWithOccludersManager.updateAndPerformOcclusionQuery(sof(1.0f));

    // If wireframe mode is on, we need to set polygon mode to GL_LINE
    if (isWireframeModeOn) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }

    // Render all objects, that passed the occlusion test
    objectsWithOccludersManager.renderAllVisibleObjects();

    // Restore normal drawing mode
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // Visualize occluders only if it's enabled
    if (visualizeOccluders) {
        objectsWithOccludersManager.renderOccluders();
    }

	// Render HUD
    hud->renderHUD(objectsWithOccludersManager.getNumObjects(), objectsWithOccludersManager.getNumVisibleObjects(), isWireframeModeOn, visualizeOccluders);
}

void OpenGLWindow027::releaseScene()
{
    skybox.reset();
    plainGround.reset();

    ObjectsWithOccludersManager::getInstance().release();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	hud.reset();
}

void OpenGLWindow027::updateScene()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
        isWireframeModeOn = !isWireframeModeOn;
    }

    if (keyPressedOnce(GLFW_KEY_C)) {
        visualizeOccluders = !visualizeOccluders;
    }

    // Update camera
    int posX, posY, width, height;
    glfwGetWindowPos(getWindow(), &posX, &posY);
    glfwGetWindowSize(getWindow(), &width, &height);
    flyingCamera.setWindowCenterPosition(glm::i32vec2(posX + width / 2, posY + height / 2));

    flyingCamera.update([this](int keyCode) {return this->keyPressed(keyCode); },
        [this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
        [this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
        [this](float f) {return this->sof(f); });
}
