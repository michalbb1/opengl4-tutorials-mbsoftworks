// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Project
#include "028-framebuffer-3D-color-picking.h"
#include "HUD028.h"
#include "objectPicker.h"
#include "world.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/static_meshes_3D/skybox.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"

bool updateCamera = true;
FlyingCamera flyingCamera(glm::vec3(0.0f, 15.0f, 150.0f), glm::vec3(0.0f, 15.0f, 149.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f);

std::unique_ptr<static_meshes_3D::Skybox> skybox;

std::unique_ptr<HUD028> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.5f, 0.5f, 0.5f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -sqrtf(2.0f) / 2.0f, -sqrtf(2.0f) / 2.0f), 0.5f);

bool visualizeColorFrameBuffer = false;

void OpenGLWindow028::initializeScene()
{
	try
	{
   		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

        sm.loadVertexShader("tut028_main", "data/shaders/tut021-specular-highlight/shader.vert");
        sm.loadFragmentShader("tut028_main", "data/shaders/tut021-specular-highlight/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
        sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
        sm.loadVertexShader("single-color", "data/shaders/single-color/shader.vert");
        sm.loadFragmentShader("single-color", "data/shaders/single-color/shader.frag");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut028_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut028_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));

        auto& singleColorShaderProgram = spm.createShaderProgram("single-color");
        singleColorShaderProgram.addShaderToProgram(sm.getVertexShader("single-color"));
        singleColorShaderProgram.addShaderToProgram(sm.getFragmentShader("single-color"));
		
        skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/jajlake1", "jpg");
		hud = std::make_unique<HUD028>(*this);
		
		SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
        tm.loadTexture2D("grass", "data/textures/grass.jpg");
        tm.loadTexture2D("crate", "data/textures/crate.png");
        tm.loadTexture2D("diamond", "data/textures/diamond.png");
        tm.loadTexture2D("white_marble", "data/textures/white_marble.jpg");
        tm.loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");
		
        ObjectPicker::getInstance().initialize();
        World::getInstance().initialize();

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

void OpenGLWindow028::renderScene()
{
	const auto& spm = ShaderProgramManager::getInstance();
	const auto& tm = TextureManager::getInstance();
	auto& mm = MatrixManager::getInstance();

    FrameBuffer::Default::bindAsBothReadAndDraw();
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
    World::getInstance().renderGround(mainProgram);

    // Render mini-maze made out of crates
    tm.getTexture("crate").bind();
    World::getInstance().renderCratesMaze(mainProgram);

    // Render all pickable objects
    auto& objectPicker = ObjectPicker::getInstance();
    objectPicker.renderAllPickableObjects();

    // Visualize frame buffer only if it's enabled
    if (visualizeColorFrameBuffer)
    {
        const auto cursorPosition = getOpenGLCursorPosition();
        objectPicker.performObjectPicking(cursorPosition.x, cursorPosition.y);
        objectPicker.copyColorToDefaultFrameBuffer();
    }

    // Rebind default framebuffer and set full viewport again
    FrameBuffer::Default::bindAsBothReadAndDraw();
    FrameBuffer::Default::setFullViewport();

	// Render HUD
    hud->renderHUD(objectPicker.isPixelPerfectSelectionMode(), visualizeColorFrameBuffer, objectPicker.getSelectedObjectDescription(), updateCamera);
}

void OpenGLWindow028::releaseScene()
{
    skybox.reset();

    ObjectPicker::getInstance().release();
    World::getInstance().release();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	hud.reset();
}

void OpenGLWindow028::onWindowSizeChanged(int width, int height)
{
    ObjectPicker::getInstance().resizePickingFrameBuffer(width, height);
}

void OpenGLWindow028::onMouseButtonPressed(int button, int action)
{
    if(visualizeColorFrameBuffer)
    {
        // If we show color frame buffer, we perform picking nonstop, so don't even react to mouse action
        return;
    }

    // Otherwise perform object picking on left mouse button press
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        const auto cursorPosition = getOpenGLCursorPosition();
        ObjectPicker::getInstance().performObjectPicking(cursorPosition.x, cursorPosition.y);
    }
}

void OpenGLWindow028::updateScene()
{
	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
        ObjectPicker::getInstance().togglePixelPerfectSelectionMode();
    }

    if (keyPressedOnce(GLFW_KEY_C)) {
        visualizeColorFrameBuffer = !visualizeColorFrameBuffer;
    }

    if(keyPressedOnce(GLFW_KEY_U)) {
        updateCamera = !updateCamera;
    }

    ObjectPicker::getInstance().updateAllPickableObjects(sof(1.0f));

    if (updateCamera)
    {
        // Update camera
        int width, height;
        glfwGetWindowSize(getWindow(), &width, &height);
        flyingCamera.setWindowCenterPosition(glm::i32vec2(width / 2, height / 2));

        flyingCamera.update([this](int keyCode) {return this->keyPressed(keyCode); },
            [this]() {double curPosX, curPosY; glfwGetCursorPos(this->getWindow(), &curPosX, &curPosY); return glm::u32vec2(curPosX, curPosY); },
            [this](const glm::i32vec2& pos) {glfwSetCursorPos(this->getWindow(), pos.x, pos.y); },
            [this](float f) {return this->sof(f); });
    }
}
