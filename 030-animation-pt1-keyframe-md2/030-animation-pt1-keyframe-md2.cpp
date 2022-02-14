// STL
#include <iostream>
#include <memory>

// GLM
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>

// Project
#include "030-animation-pt1-keyframe-md2.h"
#include "HUD030.h"
#include "modelCollection.h"

#include "../common_classes/flyingCamera.h"

#include "../common_classes/freeTypeFont.h"
#include "../common_classes/shaderManager.h"
#include "../common_classes/shaderProgramManager.h"
#include "../common_classes/textureManager.h"
#include "../common_classes/samplerManager.h"
#include "../common_classes/matrixManager.h"

#include "../common_classes/frameBuffer.h"
#include "../common_classes/static_meshes_3D/plainGround.h"
#include "../common_classes/static_meshes_3D/skybox.h"

#include "../common_classes/shader_structs/ambientLight.h"
#include "../common_classes/shader_structs/diffuseLight.h"
#include "../common_classes/shader_structs/material.h"

namespace opengl4_mbsoftworks {
namespace tutorial030 {

bool updateCamera = true;
FlyingCamera flyingCamera(glm::vec3(0.0f, 15.0f, 150.0f), glm::vec3(0.0f, 15.0f, 149.0f), glm::vec3(0.0f, 1.0f, 0.0f), 75.0f);

std::unique_ptr<static_meshes_3D::Skybox> skybox;
std::unique_ptr<HUD030> hud;

shader_structs::AmbientLight ambientLight(glm::vec3(0.5f, 0.5f, 0.5f));
shader_structs::DiffuseLight diffuseLight(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.0f, -sqrtf(2.0f) / 2.0f, -sqrtf(2.0f) / 2.0f), 0.5f);

std::unique_ptr<static_meshes_3D::PlainGround> plainGround; // Plain ground used to render ground

void OpenGLWindow030::initializeScene()
{
	try
	{
   		auto& sm = ShaderManager::getInstance();
		auto& spm = ShaderProgramManager::getInstance();
		auto& tm = TextureManager::getInstance();

        sm.loadVertexShader("tut030_main", "data/shaders/tut021-specular-highlight/shader.vert");
        sm.loadFragmentShader("tut030_main", "data/shaders/tut021-specular-highlight/shader.frag");
		sm.loadFragmentShader(ShaderKeys::ambientLight(), "data/shaders/lighting/ambientLight.frag");
        sm.loadFragmentShader(ShaderKeys::diffuseLight(), "data/shaders/lighting/diffuseLight.frag");
        sm.loadFragmentShader(ShaderKeys::specularHighlight(), "data/shaders/lighting/specularHighlight.frag");
        sm.loadVertexShader("single-color", "data/shaders/single-color/shader.vert");
        sm.loadFragmentShader("single-color", "data/shaders/single-color/shader.frag");

        sm.loadVertexShader("md2", "data/shaders/tut030-animation-pt1-keyframe-md2/md2anim.vert");

		auto& mainShaderProgram = spm.createShaderProgram("main");
		mainShaderProgram.addShaderToProgram(sm.getVertexShader("tut030_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader("tut030_main"));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
		mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        mainShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));

        auto& md2AnimationShaderProgram = spm.createShaderProgram("md2");
        md2AnimationShaderProgram.addShaderToProgram(sm.getVertexShader("md2"));
        md2AnimationShaderProgram.addShaderToProgram(sm.getFragmentShader("tut030_main"));
        md2AnimationShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::ambientLight()));
        md2AnimationShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::diffuseLight()));
        md2AnimationShaderProgram.addShaderToProgram(sm.getFragmentShader(ShaderKeys::specularHighlight()));
		
        skybox = std::make_unique<static_meshes_3D::Skybox>("data/skyboxes/snowy_mountains", "png");
		hud = std::make_unique<HUD030>(*this);
		
        SamplerManager::getInstance().createSampler("main", MAG_FILTER_BILINEAR, MIN_FILTER_TRILINEAR);
        SamplerManager::getInstance().createSampler("framebuffer", MAG_FILTER_BILINEAR, MIN_FILTER_BILINEAR);
        tm.loadTexture2D("snow", "data/textures/snow.png");
        tm.loadTexture2D("wood", "data/textures/wood.jpg");
        tm.loadTexture2D("prismarine_dark", "data/textures/prismarine_dark.png");
        tm.loadTexture2D("diamond", "data/textures/diamond.png");
        tm.loadTexture2D("white_marble", "data/textures/white_marble.jpg");
        tm.loadTexture2D("scifi_metal", "data/textures/scifi_metal.jpg");

        plainGround = std::make_unique<static_meshes_3D::PlainGround>(true, true, true);
        ModelCollection::getInstance().initialize();

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

void OpenGLWindow030::renderScene()
{
    const auto& tm = TextureManager::getInstance();
    const auto& spm = ShaderProgramManager::getInstance();
	auto& mm = MatrixManager::getInstance();
    const auto& modelCollection = ModelCollection::getInstance();

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

    // Render snowy ground
    mainProgram.setModelAndNormalMatrix(glm::mat4(1.0f));
    tm.getTexture("snow").bind();
    plainGround->render();

    // Setup MD2 model shader program and render the model collection
    auto& md2Program = spm.getShaderProgram("md2");
    md2Program.useProgram();
    md2Program[ShaderConstants::projectionMatrix()] = getProjectionMatrix();
    md2Program[ShaderConstants::viewMatrix()] = flyingCamera.getViewMatrix();
    md2Program[ShaderConstants::sampler()] = 0;
    ambientLight.setUniform(md2Program, ShaderConstants::ambientLight());
    diffuseLight.setUniform(md2Program, ShaderConstants::diffuseLight());
    modelCollection.renderModels(md2Program);

	// Render HUD
    hud->renderHUD();
}

void OpenGLWindow030::releaseScene()
{
    skybox.reset();
    plainGround.reset();
    ModelCollection::getInstance().release();

	ShaderManager::getInstance().clearShaderCache();
	ShaderProgramManager::getInstance().clearShaderProgramCache();
	TextureManager::getInstance().clearTextureCache();
	SamplerManager::getInstance().clearSamplerCache();
	FreeTypeFontManager::getInstance().clearFreeTypeFontCache();

	hud.reset();
}

void OpenGLWindow030::updateScene()
{
    const auto deltaTime = sof(1.0f);
    auto& world = ModelCollection::getInstance();

	if (keyPressedOnce(GLFW_KEY_ESCAPE)) {
		closeWindow();
	}

    if (keyPressedOnce(GLFW_KEY_F3)) {
        setVerticalSynchronization(!isVerticalSynchronizationEnabled());
    }

    if (keyPressedOnce(GLFW_KEY_Q)) {
        ModelCollection::getInstance().toggleAnimationLooping();
    }

    if (keyPressedOnce(GLFW_KEY_KP_ADD)) {
        ModelCollection::getInstance().nextAnimation();
    }

    if (keyPressedOnce(GLFW_KEY_KP_SUBTRACT)) {
        ModelCollection::getInstance().previousAnimation();
    }

    if (keyPressedOnce(GLFW_KEY_X)) {
       ModelCollection::getInstance().nextModel();
    }

    if (keyPressedOnce(GLFW_KEY_Z)) {
       ModelCollection::getInstance().previousModel();
    }

    world.updateAnimations(deltaTime);

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

} // namespace tutorial 030
} // namespace opengl4_mbsoftworks